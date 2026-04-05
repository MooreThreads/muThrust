/******************************************************************************
 * Copyright (c) 2016, NVIDIA CORPORATION.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the NVIDIA CORPORATION nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/
#pragma once

// 平台检测宏
#if defined(__MUSACC_VER_MAJOR__)
  #define THRUST_MUSA_ENABLED 1
  #define THRUST_CUDA_ENABLED 0
#elif defined(__CUDACC__) || defined(_NVHPC_CUDA)
  #define THRUST_MUSA_ENABLED 0
  #define THRUST_CUDA_ENABLED 1
#else
  #define THRUST_MUSA_ENABLED 0
  #define THRUST_CUDA_ENABLED 0
#endif

// CDP (MUSA Dynamic Parallelism) 支持
// MUSA 平台暂不支持 CDP
#if THRUST_MUSA_ENABLED
  #define THRUST_CDP_ENABLED 0
#elif defined(__CUDACC_RDC__) && defined(__CUDACC__)
  #if !defined(__CUDA_ARCH__) || (__CUDA_ARCH__ >= 350)
    #define THRUST_CDP_ENABLED 1
  #else
    #define THRUST_CDP_ENABLED 0
  #endif
#else
  #define THRUST_CDP_ENABLED 0
#endif

// ============================================================================
// 统一 API 宏：根据平台自动展开
// ============================================================================

#if THRUST_MUSA_ENABLED
  #define THRUST_API_PREFIX(name) musa##name
#else
  #define THRUST_API_PREFIX(name) cuda##name
#endif

// ----------------------------------------------------------------------------
// 运行时 API 函数映射
// ----------------------------------------------------------------------------

// 内存管理
#define THRUST_MALLOC           THRUST_API_PREFIX(Malloc)
#define THRUST_FREE             THRUST_API_PREFIX(Free)
#define THRUST_MEMCPY           THRUST_API_PREFIX(Memcpy)
#define THRUST_MEMCPY_ASYNC     THRUST_API_PREFIX(MemcpyAsync)
#define THRUST_MEMSET           THRUST_API_PREFIX(Memset)
#define THRUST_MEMSET_ASYNC     THRUST_API_PREFIX(MemsetAsync)
#define THRUST_MEM_GET_INFO     THRUST_API_PREFIX(MemGetInfo)
#define THRUST_HOST_ALLOC       THRUST_API_PREFIX(HostAlloc)
#define THRUST_HOST_FREE        THRUST_API_PREFIX(FreeHost)

// 设备管理
#define THRUST_GET_DEVICE_COUNT THRUST_API_PREFIX(GetDeviceCount)
#define THRUST_SET_DEVICE       THRUST_API_PREFIX(SetDevice)
#define THRUST_GET_DEVICE       THRUST_API_PREFIX(GetDevice)
#define THRUST_GET_DEVICE_PROP  THRUST_API_PREFIX(GetDeviceProperties)
#define THRUST_DEVICE_RESET     THRUST_API_PREFIX(DeviceReset)
#define THRUST_DEVICE_SYNC      THRUST_API_PREFIX(DeviceSynchronize)

// 流管理
#define THRUST_STREAM_T         THRUST_API_PREFIX(Stream_t)
#define THRUST_STREAM_CREATE    THRUST_API_PREFIX(StreamCreate)
#define THRUST_STREAM_CREATE_WITH_FLAGS THRUST_API_PREFIX(StreamCreateWithFlags)
#define THRUST_STREAM_DESTROY   THRUST_API_PREFIX(StreamDestroy)
#define THRUST_STREAM_SYNC      THRUST_API_PREFIX(StreamSynchronize)
#define THRUST_STREAM_WAIT_EVENT THRUST_API_PREFIX(StreamWaitEvent)

// 事件管理
#define THRUST_EVENT_T          THRUST_API_PREFIX(Event_t)
#define THRUST_EVENT_CREATE     THRUST_API_PREFIX(EventCreate)
#define THRUST_EVENT_CREATE_WITH_FLAGS THRUST_API_PREFIX(EventCreateWithFlags)
#define THRUST_EVENT_DESTROY    THRUST_API_PREFIX(EventDestroy)
#define THRUST_EVENT_RECORD     THRUST_API_PREFIX(EventRecord)
#define THRUST_EVENT_SYNC       THRUST_API_PREFIX(EventSynchronize)
#define THRUST_EVENT_ELAPSED_TIME THRUST_API_PREFIX(EventElapsedTime)

// 错误处理
#define THRUST_GET_LAST_ERROR   THRUST_API_PREFIX(GetLastError)
#define THRUST_PEEK_AT_LAST_ERROR THRUST_API_PREFIX(PeekAtLastError)
#define THRUST_GET_ERROR_STRING THRUST_API_PREFIX(GetErrorString)
#define THRUST_GET_ERROR_NAME   THRUST_API_PREFIX(GetErrorName)

// 内存拷贝类型
#define THRUST_MEMCPY_H2D       THRUST_API_PREFIX(MemcpyHostToDevice)
#define THRUST_MEMCPY_D2H       THRUST_API_PREFIX(MemcpyDeviceToHost)
#define THRUST_MEMCPY_D2D       THRUST_API_PREFIX(MemcpyDeviceToDevice)
#define THRUST_MEMCPY_DEFAULT   THRUST_API_PREFIX(MemcpyDefault)

// 错误类型
#define THRUST_SUCCESS          THRUST_API_PREFIX(Success)
#define THRUST_ERROR_T          THRUST_API_PREFIX(Error_t)

// 流标志
#define THRUST_STREAM_DEFAULT   THRUST_API_PREFIX(StreamDefault)
#define THRUST_STREAM_NON_BLOCKING THRUST_API_PREFIX(StreamNonBlocking)

// 事件标志
#define THRUST_EVENT_DEFAULT    THRUST_API_PREFIX(EventDefault)
#define THRUST_EVENT_DISABLE_TIMING THRUST_API_PREFIX(EventDisableTiming)

// ----------------------------------------------------------------------------
// 设备端内置函数映射
// ----------------------------------------------------------------------------

#if THRUST_MUSA_ENABLED

// Warp shuffle
#define THRUST_SHFL_DOWN_SYNC(mask, var, delta, width) \
  __musa_shfl_down_sync(mask, var, delta, width)
#define THRUST_SHFL_UP_SYNC(mask, var, delta, width) \
  __musa_shfl_up_sync(mask, var, delta, width)
#define THRUST_SHFL_XOR_SYNC(mask, var, lane_mask, width) \
  __musa_shfl_xor_sync(mask, var, lane_mask, width)
#define THRUST_SHFL_SYNC(mask, var, src_lane, width) \
  __musa_shfl_sync(mask, var, src_lane, width)

// 内存屏障
#define THRUST_THREADFENCE()          __musa_threadfence()
#define THRUST_THREADFENCE_BLOCK()    __musa_threadfence_block()
#define THRUST_THREADFENCE_SYSTEM()   __musa_threadfence_system()

// 同步
#define THRUST_SYNCTHREADS()          __musa_syncthreads()
#define THRUST_SYNCTHREADS_COUNT(p)   __musa_syncthreads_count(p)
#define THRUST_SYNCTHREADS_AND(p)     __musa_syncthreads_and(p)
#define THRUST_SYNCTHREADS_OR(p)      __musa_syncthreads_or(p)

// Warp vote
#define THRUST_BALLOT_SYNC(mask, p)   __musa_ballot_sync(mask, p)
#define THRUST_ALL_SYNC(mask, p)      __musa_all_sync(mask, p)
#define THRUST_ANY_SYNC(mask, p)      __musa_any_sync(mask, p)

// 交叉线程
#define THRUST_MATCH_ALL_SYNC(mask, value, pred) __musa_match_all_sync(mask, value, pred)
#define THRUST_MATCH_ANY_SYNC(mask, value)       __musa_match_any_sync(mask, value)

#else // MUSA

// Warp shuffle
#define THRUST_SHFL_DOWN_SYNC(mask, var, delta, width) \
  __shfl_down_sync(mask, var, delta, width)
#define THRUST_SHFL_UP_SYNC(mask, var, delta, width) \
  __shfl_up_sync(mask, var, delta, width)
#define THRUST_SHFL_XOR_SYNC(mask, var, lane_mask, width) \
  __shfl_xor_sync(mask, var, lane_mask, width)
#define THRUST_SHFL_SYNC(mask, var, src_lane, width) \
  __shfl_sync(mask, var, src_lane, width)

// 内存屏障
#define THRUST_THREADFENCE()          __threadfence()
#define THRUST_THREADFENCE_BLOCK()    __threadfence_block()
#define THRUST_THREADFENCE_SYSTEM()   __threadfence_system()

// 同步
#define THRUST_SYNCTHREADS()          __syncthreads()
#define THRUST_SYNCTHREADS_COUNT(p)   __syncthreads_count(p)
#define THRUST_SYNCTHREADS_AND(p)     __syncthreads_and(p)
#define THRUST_SYNCTHREADS_OR(p)      __syncthreads_or(p)

// Warp vote
#define THRUST_BALLOT_SYNC(mask, p)   __ballot_sync(mask, p)
#define THRUST_ALL_SYNC(mask, p)      __all_sync(mask, p)
#define THRUST_ANY_SYNC(mask, p)      __any_sync(mask, p)

// 交叉线程
#define THRUST_MATCH_ALL_SYNC(mask, value, pred) __match_all_sync(mask, value, pred)
#define THRUST_MATCH_ANY_SYNC(mask, value)       __match_any_sync(mask, value)

#endif // THRUST_MUSA_ENABLED

// ----------------------------------------------------------------------------
// 架构宏
// ----------------------------------------------------------------------------

#if THRUST_MUSA_ENABLED
  #define THRUST_ARCH_MACRO __MUSA_ARCH__
#else
  #define THRUST_ARCH_MACRO __CUDA_ARCH__
#endif

// CDP launch
#if THRUST_CDP_ENABLED
  #if THRUST_MUSA_ENABLED
    #define THRUST_GET_PARAMETER_BUFFER(alignment, size) musaGetParameterBuffer(alignment, size)
    #define THRUST_LAUNCH_DEVICE(kernel, params, grid, block, smem, stream) \
      musaLaunchDevice((void*)kernel, params, grid, block, smem, stream)
  #else
    #define THRUST_GET_PARAMETER_BUFFER(alignment, size) cudaGetParameterBuffer(alignment, size)
    #define THRUST_LAUNCH_DEVICE(kernel, params, grid, block, smem, stream) \
      cudaLaunchDevice((void*)kernel, params, grid, block, smem, stream)
  #endif
#else
  // CDP 禁用时提供空实现
  #define THRUST_GET_PARAMETER_BUFFER(alignment, size) ((void*)nullptr)
  #define THRUST_LAUNCH_DEVICE(kernel, params, grid, block, smem, stream) \
    THRUST_API_PREFIX(ErrorNotSupported)
#endif
