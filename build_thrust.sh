#!/bin/bash
# Thrust MUSA 构建脚本
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# 默认参数
JOBS=$(nproc)
TEST_JOBS=8
GPUS="0,1,2,3,4,5,6,7"
CLEAN_BUILD=false
BUILD_ONLY=false

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
            GPUS="$2"
            shift 2
            ;;
        -n|--no-clean)
            CLEAN_BUILD=false
            shift
            ;;
        --clean)
            CLEAN_BUILD=true
            shift
            ;;
        build)
            BUILD_ONLY=true
            shift
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# 清理
if [ "$CLEAN_BUILD" = true ]; then
    echo "Cleaning build directory..."
    rm -rf build
fi

# 创建构建目录
mkdir -p build

# CMake 配置
echo "Configuring Thrust with MUSA..."
cmake -B build \
    -DMUSA_64_BIT_DEVICE_CODE=ON \
    -DTHRUST_ENABLE_TESTING=ON \
    -DTHRUST_ENABLE_EXAMPLES=OFF \
    -DTHRUST_ENABLE_HEADER_TESTING=OFF

# 编译
echo "Building Thrust ($JOBS parallel jobs)..."
cmake --build build -j"$JOBS"

# 测试
if [ "$BUILD_ONLY" = false ]; then
    echo "Running tests on GPUs: $GPUS"
    export MUSA_VISIBLE_DEVICES="$GPUS"
    ctest --test-dir build -j"$TEST_JOBS" --output-on-failure
fi

echo "Done!"
