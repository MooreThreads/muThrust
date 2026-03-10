# Thrust MUSA 移植进度

## 当前状态：调试运行时错误

**最后更新时间**: 2026-03-08

## 已完成的工作

### 1. 条件编译宏系统 ✅
- 创建了 `thrust/system/cuda/detail/platform_macros.h`
  - `THRUST_MUSA_ENABLED` / `THRUST_CUDA_ENABLED` 平台检测
  - `THRUST_CDP_ENABLED` CDP 支持检测
  - 统一 API 宏：`THRUST_MALLOC`, `THRUST_FREE`, `THRUST_MEMCPY` 等
  - 设备端内置函数映射：`THRUST_SHFL_DOWN_SYNC`, `THRUST_SYNCTHREADS` 等
  - 架构宏：`THRUST_ARCH_MACRO`

### 2. CMake 配置 ✅
- 修改 `cmake/ThrustCudaConfig.cmake`
  - MUSA 架构检测：mp_21, mp_30, mp_31
  - MUSA 架构标志：`--offload-arch=mp_XX`
  - CUDA 架构标志保持不变

### 3. 头文件条件包含 ✅
- `thrust/system/cuda/detail/guarded_cuda_runtime_api.h`
- `thrust/system/cuda/detail/guarded_driver_types.h`
- 根据平台自动选择 `musa_runtime_api.h` 或 `cuda_runtime_api.h`

### 4. CDP 禁用 ✅
- `thrust/system/cuda/config.h` 中添加 MUSA 平台 CDP 强制禁用逻辑
- `THRUST_MUSA_DISABLE_CDP` 宏定义

### 5. 核心文件 API 转换 ✅
- `thrust/system/cuda/detail/parallel_for.h` - musaError_t, musaStream_t
- `thrust/system/cuda/detail/core/triple_chevron_launch.h` - 完整 MUSA API
- `thrust/system/cuda/detail/core/util.h` - MUSA 架构调优选择
- `thrust/system/cuda/detail/core/agent_launcher.h` - `__MUSA_ARCH__` 支持
- `thrust/system/cuda/detail/future.inl` - musaStreamQuery

### 6. 测试框架 ✅
- `testing/CMakeLists.txt` - 使用 `musa_add_executable`
- `testing/unittest/CMakeLists.txt` - 使用 `musa_add_library`
- `testing/unittest/cuda/testframework.cu` - MUSA API 调用

### 7. CUB 库适配 ✅ (在 cub/ 目录)
- `cub/util_arch.cuh` - MUSA 架构检测
- `cub/detail/device_synchronize.cuh` - MUSA API
- `cub/util_device.cuh` - PtxVersion, SmVersion, SyncStream

## 当前问题

### 运行时错误：musaErrorInvalidDeviceFunction

**错误信息**:
```
Testing Device 0: "MTT S5000"
Running 14 unit tests....
terminate called after throwing an instance of 'thrust::system::system_error'
  what():  parallel_for failed: musaErrorInvalidDeviceFunction: invalid device function
```

**编译确认正确**:
- MUSA 编译器: `/usr/local/musa/bin/mcc`
- 架构标志: `--offload-arch=mp_31`
- 包含路径正确指向 MUSA 头文件

**可能的原因**:
1. 内核函数未正确编译为设备代码
2. 架构版本不匹配 (MUSA mp_31 vs Thrust/CUB 内部架构 300/350/520/600)
3. 模板实例化问题
4. CUB BlockReduce 等组件需要额外的 MUSA 适配

## 待调查方向

### 高优先级
1. **检查 CUB BlockReduce 模板参数**
   - `cub::BlockReduce<T, BLOCK_THREADS, ALGORITHM, 1, 1, Arch::ver>`
   - `Arch::ver` 对 MUSA 可能返回不正确的值

2. **检查内核符号导出**
   - 确认 `__global__` 内核是否正确编译
   - 检查模板实例化

3. **简化测试**
   - 创建最小化 MUSA 内核测试
   - 验证基本内核启动功能

### 中优先级
1. **CUB 设备算法**
   - `cub/device/device_reduce.cuh` 可能需要适配
   - 检查 `cub::DispatchReduce` 等调度逻辑

2. **Warp 级原语**
   - 检查 `__musa_shfl_down_sync` 等是否正确实现
   - 验证 warp 大小 (MUSA 可能不同)

## 文件修改清单

### Thrust 核心文件
| 文件 | 状态 | 说明 |
|------|------|------|
| `thrust/system/cuda/detail/platform_macros.h` | ✅ 新建 | 统一 API 宏 |
| `thrust/system/cuda/config.h` | ✅ 修改 | CDP 禁用 |
| `thrust/system/cuda/detail/guarded_cuda_runtime_api.h` | ✅ 修改 | 条件包含 |
| `thrust/system/cuda/detail/guarded_driver_types.h` | ✅ 修改 | 条件包含 |
| `thrust/system/cuda/detail/parallel_for.h` | ✅ 修改 | MUSA API |
| `thrust/system/cuda/detail/core/triple_chevron_launch.h` | ✅ 修改 | MUSA API |
| `thrust/system/cuda/detail/core/util.h` | ✅ 修改 | MUSA 架构调优 |
| `thrust/system/cuda/detail/core/agent_launcher.h` | ✅ 修改 | `__MUSA_ARCH__` |
| `thrust/system/cuda/detail/future.inl` | ✅ 修改 | musaStreamQuery |

### CMake 文件
| 文件 | 状态 | 说明 |
|------|------|------|
| `cmake/ThrustCudaConfig.cmake` | ✅ 修改 | MUSA 架构配置 |
| `testing/CMakeLists.txt` | ✅ 修改 | musa_add_executable |
| `testing/unittest/CMakeLists.txt` | ✅ 修改 | musa_add_library |

### 测试框架
| 文件 | 状态 | 说明 |
|------|------|------|
| `testing/unittest/cuda/testframework.cu` | ✅ 修改 | MUSA API |

### CUB 文件 (cub/ 目录)
| 文件 | 状态 | 说明 |
|------|------|------|
| `cub/util_arch.cuh` | ✅ 修改 | MUSA 架构检测 |
| `cub/detail/device_synchronize.cuh` | ✅ 修改 | MUSA API |
| `cub/util_device.cuh` | ✅ 修改 | PtxVersion 等 |

## 下一步行动

1. 创建最小化测试验证 MUSA 内核启动
2. 检查 `parallel_for` 中的 `ParallelForKernel` 是否正确编译
3. 验证 CUB BlockReduce 的模板参数
4. 如需要，检查 CUB 设备算法的调度逻辑

## 构建命令

```bash
cd /data/mingxu/src/cub_1.17/thrust
cmake -DMUSA_64_BIT_DEVICE_CODE=ON -B build
cmake --build build --target thrust.test.reduce -j$(nproc)
./build/bin/thrust.test.reduce
```

## 环境信息

- GPU: MTT S5000 (mp_31)
- MUSA: /usr/local/musa
- 编译器: /usr/local/musa/bin/mcc
- 架构: mp_21, mp_30, mp_31
