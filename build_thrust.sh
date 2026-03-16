#!/bin/bash
# Thrust 并行编译和测试脚本 (使用 Ninja)
#
# 所有参数均可通过环境变量设置，命令行参数优先级更高
# 环境变量:
#   THRUST_JOBS          编译并行数 (默认: nproc)
#   THRUST_TEST_JOBS     测试并行数 (默认: 1)
#   THRUST_MUSA_ARCH     MUSA 目标架构 (默认: mp_31)
#   THRUST_MUSA_DEVICES  设置 MUSA_VISIBLE_DEVICES
#   THRUST_NO_CLEAN      设置为 1 不删除 build 目录
#   THRUST_EXCLUDE_TESTS 排除匹配正则表达式的测试
#   THRUST_BUILD_ONLY    设置为 1 仅编译不测试

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
THRUST_DIR="${SCRIPT_DIR}"
BUILD_DIR="${THRUST_DIR}/build"
SOURCE_DIR="${THRUST_DIR}"

# 默认值 - 可通过环境变量覆盖
JOBS="${THRUST_JOBS:-$(nproc)}"
TEST_JOBS="${THRUST_TEST_JOBS:-1}"
RUN_TEST=true
TEST_VERBOSE="-V"
MUSA_DEVICES="${THRUST_MUSA_DEVICES:-}"  # 默认所有GPU可见
LOG_FILE="${THRUST_DIR}/test_verbose.log"
REPORT_FILE="${THRUST_DIR}/test_report.md"
SKIP_CLEAN="${THRUST_NO_CLEAN:-false}"
BUILD_ONLY="${THRUST_BUILD_ONLY:-false}"
EXCLUDE_TESTS="${THRUST_EXCLUDE_TESTS:-namespace_wrapped|unittest|test__cpp_complex}"  # 默认排除的测试用例
MUSA_ARCH="${THRUST_MUSA_ARCH:-mp_31}"  # 默认 MUSA 架构

# CUB 相关 - Thrust CUDA/MUSA 后端依赖 CUB
CUB_REPO="git@sh-code.mthreads.com:sw/muAlg.git"
CUB_BRANCH="develop-1.17"
CUB_DIR="${THRUST_DIR}/dependencies/cub/cub"

show_help() {
    cat << EOF
用法: $0 [选项] [命令]

命令:
  clean         仅清理 build 目录
  build         仅编译，不运行测试

选项:
  -j, --jobs N      编译并行数 (默认: $(nproc))
  -T, --test-jobs N 测试并行数 (默认: 1)
  -g, --gpus DEVICES 设置 MUSA_VISIBLE_DEVICES (如: 0,1,2,3)
  -a, --arch ARCH   MUSA 目标架构 (默认: mp_31, 支持: mp_22, mp_31)
  -n, --no-clean    不删除 build 目录 (增量编译)
  -E, --exclude RE  排除匹配正则表达式的测试 (默认: ${EXCLUDE_TESTS})
                    传空字符串 "" 可取消默认排除
  -h, --help        显示帮助

默认行为:
  1. 检查并安装 cub (如果缺失)
  2. 删除 build 目录
  3. CMake 配置
  4. 编译
  5. 运行测试 (ctest -V)
  6. 生成 markdown 报告

架构说明:
  mp_22  - S4000 系列 (PTX 220, Warp 128 线程)
  mp_31  - S5000 系列 (PTX 310, Warp 32 线程)

示例:
  $0                          # 完整流程：清理、编译、测试、生成报告
  $0 build                    # 仅编译
  $0 -n                       # 增量编译并测试
  $0 -a mp_22 build           # 为 S4000 编译
  $0 -T 4 -g 0,1,2,3          # 用4个并行测试，只用GPU 0-3

环境变量:
  THRUST_JOBS          编译并行数
  THRUST_TEST_JOBS     测试并行数
  THRUST_MUSA_ARCH     MUSA 目标架构
  THRUST_MUSA_DEVICES  设置 MUSA_VISIBLE_DEVICES
  THRUST_NO_CLEAN      设置为 1 不删除 build 目录
  THRUST_EXCLUDE_TESTS 排除匹配正则表达式的测试
  THRUST_BUILD_ONLY    设置为 1 仅编译不测试
EOF
}

check_and_install_cub() {
    # Thrust 依赖 CUB 在 dependencies/cub/cub 目录 (thrust/cub 符号链接指向这里)
    if [ -d "${CUB_DIR}" ] && [ "$(ls -A ${CUB_DIR} 2>/dev/null)" ]; then
        echo "CUB 已存在: ${CUB_DIR}"
        return 0
    fi

    echo "检测到 CUB 未安装，正在从 ${CUB_REPO} 下载..."

    TEMP_DIR=$(mktemp -d)
    trap "rm -rf ${TEMP_DIR}" EXIT

    cd "${TEMP_DIR}"
    git clone --depth 1 --branch "${CUB_BRANCH}" "${CUB_REPO}" muAlg

    echo "正在安装 CUB 到 ${CUB_DIR}..."
    mkdir -p "${THRUST_DIR}/dependencies/cub"
    cp -r muAlg/cub "${THRUST_DIR}/dependencies/cub/"

    echo "CUB 安装完成"
    cd "${THRUST_DIR}"
}

# 解析参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        -T|--test-jobs)
            TEST_JOBS="$2"
            shift 2
            ;;
        -g|--gpus)
            MUSA_DEVICES="$2"
            shift 2
            ;;
        -a|--arch)
            MUSA_ARCH="$2"
            shift 2
            ;;
        -E|--exclude)
            EXCLUDE_TESTS="$2"
            shift 2
            ;;
        -n|--no-clean)
            SKIP_CLEAN=true
            shift
            ;;
        clean)
            echo "清理 build 目录..."
            rm -rf "${BUILD_DIR}"
            exit 0
            ;;
        build)
            BUILD_ONLY=true
            RUN_TEST=false
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            echo "未知选项: $1"
            show_help
            exit 1
            ;;
    esac
done

# 1. 检查并安装 cub
check_and_install_cub

cd "${THRUST_DIR}"

# 2. 清理 build 目录
if [ "$SKIP_CLEAN" = false ] && [ -d "${BUILD_DIR}" ]; then
    echo "删除 build 目录..."
    rm -rf "${BUILD_DIR}"
fi

# 3. CMake 配置
echo ""
echo "=========================================="
echo "CMake 配置 (Ninja)..."
echo "MUSA 架构: ${MUSA_ARCH}"
echo "=========================================="
cmake -G Ninja \
    -DMUSA_64_BIT_DEVICE_CODE=ON \
    -DMUSA_MCC_FLAGS="--offload-arch=${MUSA_ARCH}" \
    -DTHRUST_ENABLE_TESTING=ON \
    -DTHRUST_ENABLE_EXAMPLES=OFF \
    -DTHRUST_ENABLE_HEADER_TESTING=OFF \
    -S "${SOURCE_DIR}" -B build

# 4. 并行编译
echo ""
echo "=========================================="
echo "编译 (并行数: ${JOBS})..."
echo "=========================================="
cmake --build build -j "${JOBS}"

if [ "$RUN_TEST" = true ]; then
    echo ""
    echo "=========================================="
    echo "运行测试 (并行数: ${TEST_JOBS})"
    echo "=========================================="

    # 设置环境变量
    if [ -n "$MUSA_DEVICES" ]; then
        export MUSA_VISIBLE_DEVICES="$MUSA_DEVICES"
        echo "MUSA_VISIBLE_DEVICES=${MUSA_DEVICES}"
    fi

    # 运行 ctest 并保存输出
    echo "测试输出保存到: ${LOG_FILE}"
    EXCLUDE_ARG=""
    if [ -n "$EXCLUDE_TESTS" ]; then
        EXCLUDE_ARG="-E ${EXCLUDE_TESTS}"
        echo "排除测试: ${EXCLUDE_TESTS}"
    fi
    ctest --test-dir build -j "${TEST_JOBS}" ${TEST_VERBOSE} ${EXCLUDE_ARG} 2>&1 | tee "${LOG_FILE}"

    # 生成 markdown 报告
    echo ""
    echo "=========================================="
    echo "生成测试报告"
    echo "=========================================="
    python3 "${THRUST_DIR}/parse_ctest_log.py" "${LOG_FILE}" "${REPORT_FILE}"
    echo "测试报告: ${REPORT_FILE}"
fi

echo ""
echo "完成!"
