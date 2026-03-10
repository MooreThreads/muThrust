# Thrust CUDA 到 MUSA 移植计划

## 移植策略

**核心原则**：在原有 `thrust/system/cuda/` 目录下进行条件编译适配，让用户无需修改程序即可在 MUSA 平台运行。

---

## 1. 当前状态分析

### 1.1 已完成的工作
- `CMakeLists.txt`: 已添加 MUSA 基础配置
- `testing/CMakeLists.txt`: 已使用 `musa_add_executable()`
- `thrust/system/musa/`: 基础框架（config.h, tag.h, execution_policy.h）
- `thrust/system/cuda/detail/malloc_and_free.h`: 已部分适配（musaMalloc 等）

### 1.2 目录结构（保持不变）
```
thrust/system/
├── cuda/          # 在此目录下添加 MUSA 条件编译
│   ├── config.h
│   ├── detail/
│   └── ...
├── musa/          # 仅保留平台标识和别名
│   ├── config.h
│   ├── tag.h
│   └── execution_policy.h
└── ...
```

---

## 2. 移植任务清单

### 阶段一：条件编译宏定义

#### 任务 1.1：创建统一平台检测宏
**文件**: `thrust/system/cuda/config.h`

添加 MUSA/CUDA 平台自动检测：
```cpp
// 平台检测
#if defined(__MUSACC_VER_MAJOR__)
  #define THRUST_DEVICE_SYSTEM MUSA
  #define THRUST_MUSA_ENABLED 1
  // MUSA 特定宏
  #define THRUST_DEVICE_PREFIX musa
  #define THRUST_RUNTIME_PREFIX musa
#elif defined(__CUDACC__)
  #define THRUST_DEVICE_SYSTEM CUDA
  #define THRUST_CUDA_ENABLED 1
  // CUDA 特定宏
  #define THRUST_DEVICE_PREFIX cuda
  #define THRUST_RUNTIME_PREFIX cuda
#endif

// CDP 支持：MUSA 暂不支持
#if THRUST_MUSA_ENABLED
  #define THRUST_CDP_ENABLED 0
#else
  #define THRUST_CDP_ENABLED THRUST_DEVICE_CUDA_DYNAMIC_PARALLELISM
#endif
```

#### 任务 1.2：创建 API 宏映射
**文件**: `thrust/system/cuda/detail/platform_macros.h`（新建）

```cpp
#pragma once

// 统一 API 宏：根据平台自动展开
#if THRUST_MUSA_ENABLED
  #define THRUST_API_PREFIX(name) musa##name
#else
  #define THRUST_API_PREFIX(name) cuda##name
#endif

// 运行时 API 映射
#define THRUST_MALLOC           THRUST_API_PREFIX(Malloc)
#define THRUST_FREE             THRUST_API_PREFIX(Free)
#define THRUST_MEMCPY           THRUST_API_PREFIX(Memcpy)
#define THRUST_MEMSET           THRUST_API_PREFIX(Memset)
#define THRUST_STREAM_CREATE    THRUST_API_PREFIX(StreamCreate)
#define THRUST_STREAM_DESTROY   THRUST_API_PREFIX(StreamDestroy)
#define THRUST_STREAM_SYNC      THRUST_API_PREFIX(StreamSynchronize)
#define THRUST_DEVICE_SYNC      THRUST_API_PREFIX(DeviceSynchronize)
#define THRUST_GET_LAST_ERROR   THRUST_API_PREFIX(GetLastError)
#define THRUST_PEEK_ERROR       THRUST_API_PREFIX(PeekAtLastError)
#define THRUST_GET_ERROR_STRING THRUST_API_PREFIX(GetErrorString)
#define THRUST_GET_DEVICE_COUNT THRUST_API_PREFIX(GetDeviceCount)
#define THRUST_SET_DEVICE       THRUST_API_PREFIX(SetDevice)
#define THRUST_GET_DEVICE_PROP  THRUST_API_PREFIX(GetDeviceProperties)

// 类型映射
#define THRUST_ERROR_T          THRUST_API_PREFIX(Error_t)
#define THRUST_SUCCESS          THRUST_API_PREFIX(Success)
#define THRUST_STREAM_T         THRUST_API_PREFIX(Stream_t)
#define THRUST_EVENT_T          THRUST_API_PREFIX(Event_t)
#define THRUST_MEMCPY_H2D       THRUST_API_PREFIX(MemcpyHostToDevice)
#define THRUST_MEMCPY_D2H       THRUST_API_PREFIX(MemcpyDeviceToHost)
#define THRUST_MEMCPY_D2D       THRUST_API_PREFIX(MemcpyDeviceToDevice)

// 内置函数映射（设备端）
#if THRUST_MUSA_ENABLED
  #define THRUST_SHFL_DOWN_SYNC  __musa_shfl_down_sync
  #define THRUST_SHFL_UP_SYNC    __musa_shfl_up_sync
  #define THRUST_SHFL_XOR_SYNC   __musa_shfl_xor_sync
  #define THRUST_SHFL_SYNC       __musa_shfl_sync
  #define THRUST_THREADFENCE     __musa_threadfence
  #define THRUST_THREADFENCE_BLOCK __musa_threadfence_block
  #define THRUST_SYNCTHREADS     __musa_syncthreads
#else
  #define THRUST_SHFL_DOWN_SYNC  __shfl_down_sync
  #define THRUST_SHFL_UP_SYNC    __shfl_up_sync
  #define THRUST_SHFL_XOR_SYNC   __shfl_xor_sync
  #define THRUST_SHFL_SYNC       __shfl_sync
  #define THRUST_THREADFENCE     __threadfence
  #define THRUST_THREADFENCE_BLOCK __threadfence_block
  #define THRUST_SYNCTHREADS     __syncthreads
#endif
```

---

### 阶段二：CDP（动态并行）禁用处理

#### 任务 2.1：在 config.h 中禁用 CDP
**文件**: `thrust/system/cuda/config.h`

```cpp
// MUSA 平台禁用 CDP
#if THRUST_MUSA_ENABLED
  #undef THRUST_DEVICE_CUDA_DYNAMIC_PARALLELISM
  #define THRUST_DEVICE_CUDA_DYNAMIC_PARALLELISM 0
  #define THRUST_MUSA_DISABLE_CDP 1
#endif
```

#### 任务 2.2：条件编译 CDP 代码块
**涉及文件**:
- `thrust/system/cuda/detail/core/triple_chevron_launch.h`
- `thrust/system/cuda/detail/par.h`
- 其他包含 CDP 逻辑的文件

**修改模式**：
```cpp
// CDP 相关代码
#if THRUST_CDP_ENABLED
  // CDP 实现代码
  template<class K>
  cudaError_t __device__
  launch_device(K k, void* buffer) const
  {
    return cudaLaunchDevice((void*)k, buffer, ...);
  }
#else
  // MUSA: 返回不支持错误
  template<class K>
  cudaError_t __device__
  launch_device(K k, void* buffer) const
  {
    THRUST_UNUSED_VAR(k);
    THRUST_UNUSED_VAR(buffer);
    return cudaErrorNotSupported;  // 或 musaErrorNotSupported
  }
#endif
```

#### 任务 2.3：屏蔽 CDP 相关测试
**文件**: `testing/CMakeLists.txt`

添加 CDP 测试限制：
```cmake
# MUSA 平台不支持 CDP 的测试
if (THRUST_MUSA_ENABLED)
  list(APPEND cdp_excluded_tests
    async_copy_device_to_device
    async_for_each_device
    # 其他 CDP 相关测试
  )
endif()

# 在测试生成时跳过
foreach(test_src IN LISTS test_srcs)
  get_filename_component(test_name "${test_src}" NAME_WLE)
  if (test_name IN_LIST cdp_excluded_tests AND THRUST_MUSA_ENABLED)
    continue()
  endif()
  # ... 生成测试
endforeach()
```

---

### 阶段三：核心文件 API 转换

#### 任务 3.1：运行自动化转换脚本
```bash
cd /data/mingxu/src/cub_1.17
# 头文件转换
uv run python scripts/header_converter.py ./thrust/thrust/system/cuda
# libclang 深度转换
uv run python scripts/libclang_converter.py ./thrust/thrust/system/cuda
```

#### 任务 3.2：手动处理关键文件

需要检查和手动调整的文件：

| 文件 | 说明 | 修改要点 |
|------|------|----------|
| `detail/guarded_cuda_runtime_api.h` | CUDA API 包含 | 添加 MUSA 头文件条件包含 |
| `detail/guarded_driver_types.h` | 驱动类型 | 添加 MUSA 类型条件包含 |
| `detail/util.h` | 工具函数 | 流和事件 API |
| `detail/malloc_and_free.h` | 内存分配 | 已部分完成，需完善 |
| `detail/copy.h` | 拷贝操作 | cudaMemcpy 调用 |
| `detail/parallel_for.h` | 并行执行 | kernel 启动 |
| `error.h` | 错误处理 | cudaError → 统一宏 |

**示例修改**（guarded_cuda_runtime_api.h）：
```cpp
#pragma once

#if THRUST_MUSA_ENABLED
  #include <musa_runtime.h>
#else
  #include <cuda_runtime_api.h>
#endif
```

#### 任务 3.3：CMake 配置更新

**文件**: `cmake/ThrustCudaConfig.cmake`

添加 MUSA 架构支持：
```cmake
# 平台检测
if (DEFINED MUSA_FOUND AND MUSA_FOUND)
  set(THRUST_DEVICE_SYSTEM "MUSA")
  set(THRUST_KNOWN_COMPUTE_ARCHS 21 30 31)
  set(THRUST_ARCH_FLAG "--offload-arch=mp_${arch}")
else()
  set(THRUST_DEVICE_SYSTEM "CUDA")
  set(THRUST_KNOWN_COMPUTE_ARCHS 35 37 50 52 53 60 61 62 70 72 75 80 86)
  set(THRUST_ARCH_FLAG "-gencode arch=compute_${arch},code=sm_${arch}")
endif()
```

---

### 阶段四：MUSA 别名目录完善

#### 任务 4.1：完善 musa/tag.h
让 `thrust::musa::` 命名空间成为 `thrust::cuda::` 的别名：
```cpp
namespace musa {
  using tag = cuda::tag;
  using execution_policy = cuda::execution_policy;
  // ... 其他别名
}
```

#### 任务 4.2：添加必要的 MUSA 特定文件
如需要，在 `thrust/system/musa/` 下添加：
- `memory.h` - 内存别名
- `pointer.h` - 指针别名
- `vector.h` - 向量别名

---

### 阶段五：构建和测试

#### 任务 5.1：创建构建脚本
**文件**: `build_thrust.sh`

```bash
#!/bin/bash
set -e
cd /data/mingxu/src/cub_1.17/thrust

# 清理旧构建
rm -rf build
mkdir -p build

# CMake 配置
cmake -B build \
  -DMUSA_64_BIT_DEVICE_CODE=ON \
  -DTHRUST_ENABLE_TESTING=ON \
  -DTHRUST_ENABLE_EXAMPLES=OFF

# 编译
cmake --build build -j$(nproc)

# 测试
ctest --test-dir build -j 8 --output-on-failure
```

#### 任务 5.2：执行构建和测试
```bash
chmod +x build_thrust.sh
./build_thrust.sh 2>&1 | tee build.log
```

#### 任务 5.3：问题修复
根据编译/测试错误，迭代修复：
1. 遗漏的 API 转换
2. CDP 相关编译错误
3. 测试失败

---

## 3. 风险和注意事项

### 3.1 CDP 禁用注意事项
- 只屏蔽设备端 kernel 启动代码
- 主机端 kernel 启动保持正常
- 相关测试跳过但不影响其他功能
- 错误返回 `cudaErrorNotSupported` / `musaErrorNotSupported`

### 3.2 架构差异
| 平台 | 架构标识 | 编译标志 |
|------|----------|----------|
| CUDA | sm_35, sm_50, sm_60... | `-gencode arch=compute_X,code=sm_X` |
| MUSA | mp_21, mp_30, mp_31 | `--offload-arch=mp_X` |

### 3.3 PTX 汇编处理
运行 PTX 修复脚本：
```bash
uv run python scripts/fix_ptx_asm.py ./thrust/thrust/system/cuda
```

---

## 4. 预计工作量

| 阶段 | 任务数 | 预计时间 |
|------|--------|----------|
| 阶段一：条件编译宏 | 2 | 1-2 小时 |
| 阶段二：CDP 禁用 | 3 | 2-3 小时 |
| 阶段三：API 转换 | 3 | 3-4 小时 |
| 阶段四：别名完善 | 2 | 1 小时 |
| 阶段五：构建测试 | 3 | 2-4 小时 |
| **总计** | **13** | **9-14 小时** |

---

## 5. 验收标准

1. **编译通过**: `cmake --build build` 无错误
2. **用户无感知**: 用户代码无需修改即可在 MUSA 运行
3. **测试通过**: 非 CDP 测试通过率 > 90%
4. **CDP 正确禁用**: CDP 相关代码编译通过但运行时返回不支持

---

## 6. 下一步行动

请确认：
1. 条件编译策略是否符合预期
2. CDP 禁用方式是否正确
3. 是否需要调整任务优先级

确认后从 **阶段一** 开始执行。
