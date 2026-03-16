# 测试方法

## 快速开始

确保已安装 Python 和 Ninja，然后直接运行 `build_thrust.sh`：

```bash
./build_thrust.sh
```

## build_thrust.sh 用法详解

### 命令

```bash
./build_thrust.sh [选项] [命令]
```

**命令：**
- 无参数：执行完整流程（清理、编译、测试、生成报告）
- `build`：仅编译，不运行测试
- `clean`：仅清理 build 目录

### 选项

| 选项 | 说明 | 默认值 |
|------|------|--------|
| `-j, --jobs N` | 编译并行数 | `nproc` (CPU 核心数) |
| `-T, --test-jobs N` | 测试并行数 | 1 |
| `-g, --gpus DEVICES` | 设置 MUSA_VISIBLE_DEVICES | 所有 GPU 可见 |
| `-a, --arch ARCH` | MUSA 目标架构 | `mp_31` |
| `-n, --no-clean` | 不删除 build 目录（增量编译） | 否 |
| `-E, --exclude RE` | 排除匹配正则表达式的测试 | `namespace_wrapped\|unittest\|test__cpp_complex` |
| `-h, --help` | 显示帮助信息 | - |

### 架构选择

MUSA 架构决定了编译目标硬件：

| 架构 | 对应硬件 | PTX 版本 | 说明 |
|------|----------|----------|------|
| `mp_22` | S4000 系列 | 220 | Warp 128 线程 |
| `mp_31` | S5000 系列 | 310 | Warp 32 线程 |

**重要：** 编译时必须指定正确的目标架构，否则运行时会报 "invalid device function" 错误。

### 环境变量

所有参数均可通过环境变量设置，命令行参数优先级更高：

| 环境变量 | 说明 |
|----------|------|
| `THRUST_JOBS` | 编译并行数 |
| `THRUST_TEST_JOBS` | 测试并行数 |
| `THRUST_MUSA_ARCH` | MUSA 目标架构 |
| `THRUST_MUSA_DEVICES` | 设置 MUSA_VISIBLE_DEVICES |
| `THRUST_NO_CLEAN` | 设置为 1 不删除 build 目录 |
| `THRUST_EXCLUDE_TESTS` | 排除匹配正则表达式的测试 |
| `THRUST_BUILD_ONLY` | 设置为 1 仅编译不测试 |

### 示例

```bash
# 完整流程：清理、编译、测试、生成报告
./build_thrust.sh

# 仅编译（不测试）
./build_thrust.sh build

# 增量编译并测试
./build_thrust.sh -n

# 为 S4000 (mp_22) 编译
./build_thrust.sh -a mp_22 build

# 使用 8 个并行测试，指定 GPU 0-7
./build_thrust.sh -T 8 -g 0,1,2,3,4,5,6,7

# 使用环境变量配置
export THRUST_MUSA_ARCH=mp_22
export THRUST_TEST_JOBS=4
./build_thrust.sh

# 排除特定测试
./build_thrust.sh -E "reduce|sort"

# 取消默认排除（运行所有测试）
./build_thrust.sh -E ""
```

### 输出文件

- `test_verbose.log`：详细测试输出日志
- `test_report.md`：Markdown 格式的测试报告

如有问题，请联系 MingXu。

<hr>

# Thrust: The C++ Parallel Algorithms Library

<table><tr>
<th><b><a href="https://github.com/nvidia/thrust/tree/main/examples">Examples</a></b></th>
<th><b><a href="https://godbolt.org/z/rsdedW">Godbolt</a></b></th>
<th><b><a href="https://nvidia.github.io/thrust">Documentation</a></b></th>
</tr></table>

Thrust is the C++ parallel algorithms library which inspired the introduction
  of parallel algorithms to the C++ Standard Library.
Thrust's **high-level** interface greatly enhances programmer **productivity**
  while enabling performance portability between GPUs and multicore CPUs.
It builds on top of established parallel programming frameworks (such as CUDA,
  TBB, and OpenMP).
It also provides a number of general-purpose facilities similar to those found
  in the C++ Standard Library.

The NVIDIA C++ Standard Library is an open source project; it is available on
  [GitHub] and included in the NVIDIA HPC SDK and CUDA Toolkit.
If you have one of those SDKs installed, no additional installation or compiler
  flags are needed to use libcu++.

## Examples

Thrust is best learned through examples.

The following example generates random numbers serially and then transfers them
  to a parallel device where they are sorted.

```cuda
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/sort.h>
#include <thrust/copy.h>
#include <thrust/random.h>

int main() {
  // Generate 32M random numbers serially.
  thrust::default_random_engine rng(1337);
  thrust::uniform_int_distribution<int> dist;
  thrust::host_vector<int> h_vec(32 << 20);
  thrust::generate(h_vec.begin(), h_vec.end(), [&] { return dist(rng); });

  // Transfer data to the device.
  thrust::device_vector<int> d_vec = h_vec;

  // Sort data on the device.
  thrust::sort(d_vec.begin(), d_vec.end());

  // Transfer data back to host.
  thrust::copy(d_vec.begin(), d_vec.end(), h_vec.begin());
}
```

[See it on Godbolt](https://godbolt.org/z/v3fdoE)

This example demonstrates computing the sum of some random numbers in parallel:

```cuda
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/reduce.h>
#include <thrust/functional.h>
#include <thrust/random.h>

int main() {
  // Generate random data serially.
  thrust::default_random_engine rng(1337);
  thrust::uniform_real_distribution<double> dist(-50.0, 50.0);
  thrust::host_vector<double> h_vec(32 << 20);
  thrust::generate(h_vec.begin(), h_vec.end(), [&] { return dist(rng); });

  // Transfer to device and compute the sum.
  thrust::device_vector<double> d_vec = h_vec;
  double x = thrust::reduce(d_vec.begin(), d_vec.end(), 0, thrust::plus<int>());
}
```

[See it on Godbolt](https://godbolt.org/z/119jxj)

This example show how to perform such a reduction asynchronously:

```cuda
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/async/copy.h>
#include <thrust/async/reduce.h>
#include <thrust/functional.h>
#include <thrust/random.h>
#include <numeric>

int main() {
  // Generate 32M random numbers serially.
  thrust::default_random_engine rng(123456);
  thrust::uniform_real_distribution<double> dist(-50.0, 50.0);
  thrust::host_vector<double> h_vec(32 << 20);
  thrust::generate(h_vec.begin(), h_vec.end(), [&] { return dist(rng); });

  // Asynchronously transfer to the device.
  thrust::device_vector<double> d_vec(h_vec.size());
  thrust::device_event e = thrust::async::copy(h_vec.begin(), h_vec.end(),
                                               d_vec.begin());

  // After the transfer completes, asynchronously compute the sum on the device.
  thrust::device_future<double> f0 = thrust::async::reduce(thrust::device.after(e),
                                                           d_vec.begin(), d_vec.end(),
                                                           0.0, thrust::plus<double>());

  // While the sum is being computed on the device, compute the sum serially on
  // the host.
  double f1 = std::accumulate(h_vec.begin(), h_vec.end(), 0.0, thrust::plus<double>());
}
```

[See it on Godbolt](https://godbolt.org/z/rsdedW)

## Getting The Thrust Source Code

Thrust is a header-only library; there is no need to build or install the project
unless you want to run the Thrust unit tests.

The CUDA Toolkit provides a recent release of the Thrust source code in
`include/thrust`. This will be suitable for most users.

Users that wish to contribute to Thrust or try out newer features should
recursively clone the Thrust Github repository:

```
git clone --recursive https://github.com/NVIDIA/thrust.git
```

## Using Thrust From Your Project

For CMake-based projects, we provide a CMake package for use with
`find_package`. See the [CMake README](thrust/cmake/README.md) for more
information. Thrust can also be added via `add_subdirectory` or tools like
the [CMake Package Manager](https://github.com/cpm-cmake/CPM.cmake).

For non-CMake projects, compile with:
- The Thrust include path (`-I<thrust repo root>`)
- The CUB include path, if using the CUDA device system (`-I<thrust repo root>/dependencies/cub/`)
- By default, the CPP host system and CUDA device system are used.
  These can be changed using compiler definitions:
  - `-DTHRUST_HOST_SYSTEM=THRUST_HOST_SYSTEM_XXX`,
     where `XXX` is `CPP` (serial, default), `OMP` (OpenMP), or `TBB` (Intel TBB)
  - `-DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_XXX`, where `XXX` is
    `CPP`, `OMP`, `TBB`, or `CUDA` (default).

## Developing Thrust

Thrust uses the [CMake build system] to build unit tests, examples, and header
  tests.
To build Thrust as a developer, it is recommended that you use our
  containerized development system:

```bash
# Clone Thrust and CUB repos recursively:
git clone --recursive https://github.com/NVIDIA/thrust.git
cd thrust

# Build and run tests and examples:
ci/local/build.bash
```

That does the equivalent of the following, but in a clean containerized
  environment which has all dependencies installed:

```bash
# Clone Thrust and CUB repos recursively:
git clone --recursive https://github.com/NVIDIA/thrust.git
cd thrust

# Create build directory:
mkdir build
cd build

# Configure -- use one of the following:
cmake ..   # Command line interface.
ccmake ..  # ncurses GUI (Linux only).
cmake-gui  # Graphical UI, set source/build directories in the app.

# Build:
cmake --build . -j ${NUM_JOBS} # Invokes make (or ninja, etc).

# Run tests and examples:
ctest
```

By default, a serial `CPP` host system, `CUDA` accelerated device system, and
  C++14 standard are used.
This can be changed in CMake and via flags to `ci/local/build.bash`

More information on configuring your Thrust build and creating a pull request
  can be found in the [contributing section].

## Licensing

Thrust is an open source project developed on [GitHub].
Thrust is distributed under the [Apache License v2.0 with LLVM Exceptions];
  some parts are distributed under the [Apache License v2.0] and the
  [Boost License v1.0].

## CI Status

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-gpu-build/CXX_TYPE=gcc,CXX_VER=9,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-gpu-build/CXX_TYPE=gcc,CXX_VER=9,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20GCC%209%20build%20and%20device%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=11,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=11,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20GCC%2011%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=10,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=10,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20GCC%2010%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=9,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=9,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20GCC%209%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=8,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=8,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20GCC%208%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=7,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=7,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20GCC%207%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=6,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=6,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20GCC%206%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=5,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=gcc,CXX_VER=5,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20GCC%205%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=clang,CXX_VER=12,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=clang,CXX_VER=12,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20Clang%2012%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=clang,CXX_VER=11,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=clang,CXX_VER=11,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20Clang%2011%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=clang,CXX_VER=10,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=clang,CXX_VER=10,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20Clang%2010%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=clang,CXX_VER=9,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=clang,CXX_VER=9,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20Clang%209%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=clang,CXX_VER=8,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=clang,CXX_VER=8,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20Clang%208%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=clang,CXX_VER=7,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=clang,CXX_VER=7,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20Clang%207%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=icc,CXX_VER=latest,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=icc,CXX_VER=latest,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=cuda,SDK_VER=11.5.1-devel/badge/icon?subject=NVCC%2011.5.1%20%2B%20ICC%20build%20and%20host%20tests'></a>

<a href='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=nvcxx,CXX_VER=22.1,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=nvhpc,SDK_VER=22.1-devel-cuda11.5/'><img src='https://gpuci.gpuopenanalytics.com/job/nvidia/job/thrust/job/branch/job/thrust-cpu-build/CXX_TYPE=nvcxx,CXX_VER=22.1,OS_TYPE=ubuntu,OS_VER=20.04,SDK_TYPE=nvhpc,SDK_VER=22.1-devel-cuda11.5/badge/icon?subject=NVC%2B%2B%2022.1%20build%20and%20host%20tests'></a>



[GitHub]: https://github.com/nvidia/thrust

[CMake section]: https://nvidia.github.io/thrust/setup/cmake_options.html
[contributing section]: https://nvidia.github.io/thrust/contributing.html

[CMake build system]: https://cmake.org

[Apache License v2.0 with LLVM Exceptions]: https://llvm.org/LICENSE.txt
[Apache License v2.0]: https://www.apache.org/licenses/LICENSE-2.0.txt
[Boost License v1.0]: https://www.boost.org/LICENSE_1_0.txt

