# CUDA to MUSA Renaming Report

Total replacements: 1580

## Statistics by Pattern

### 1. Include 路径

Count: 290

- **dependencies/cub/cub/agent/agent_adjacent_difference.cuh:37**
  - Original: `#include <thrust/system/cuda/`
  - Replaced: `#include <thrust/system/musa/`

- **dependencies/cub/cub/agent/agent_merge_sort.cuh:37**
  - Original: `#include <thrust/system/cuda/`
  - Replaced: `#include <thrust/system/musa/`

- **dependencies/cub/cub/agent/agent_sub_warp_merge_sort.cuh:36**
  - Original: `#include <thrust/system/cuda/`
  - Replaced: `#include <thrust/system/musa/`

- **dependencies/cub/cub/device/dispatch/dispatch_adjacent_difference.cuh:37**
  - Original: `#include <thrust/system/cuda/`
  - Replaced: `#include <thrust/system/musa/`

- **dependencies/cub/cub/device/dispatch/dispatch_histogram.cuh:49**
  - Original: `#include <thrust/system/cuda/`
  - Replaced: `#include <thrust/system/musa/`

- **dependencies/cub/cub/device/dispatch/dispatch_merge_sort.cuh:35**
  - Original: `#include <thrust/system/cuda/`
  - Replaced: `#include <thrust/system/musa/`

- **dependencies/cub/cub/device/dispatch/dispatch_radix_sort.cuh:52**
  - Original: `#include <thrust/system/cuda/`
  - Replaced: `#include <thrust/system/musa/`

- **dependencies/cub/cub/device/dispatch/dispatch_reduce.cuh:49**
  - Original: `#include <thrust/system/cuda/`
  - Replaced: `#include <thrust/system/musa/`

- **dependencies/cub/cub/device/dispatch/dispatch_reduce_by_key.cuh:48**
  - Original: `#include <thrust/system/cuda/`
  - Replaced: `#include <thrust/system/musa/`

- **dependencies/cub/cub/device/dispatch/dispatch_rle.cuh:48**
  - Original: `#include <thrust/system/cuda/`
  - Replaced: `#include <thrust/system/musa/`

  ... and 280 more

### 2. 命名空间 cuda_cub

Count: 587

- **dependencies/cub/cub/agent/agent_adjacent_difference.cuh:71**
  - Original: `cuda_cub::`
  - Replaced: `musa_cub::`

- **dependencies/cub/cub/agent/agent_adjacent_difference.cuh:111**
  - Original: `cuda_cub::`
  - Replaced: `musa_cub::`

- **dependencies/cub/cub/agent/agent_merge_sort.cuh:80**
  - Original: `cuda_cub::`
  - Replaced: `musa_cub::`

- **dependencies/cub/cub/agent/agent_merge_sort.cuh:81**
  - Original: `cuda_cub::`
  - Replaced: `musa_cub::`

- **dependencies/cub/cub/agent/agent_merge_sort.cuh:387**
  - Original: `cuda_cub::`
  - Replaced: `musa_cub::`

- **dependencies/cub/cub/agent/agent_merge_sort.cuh:388**
  - Original: `cuda_cub::`
  - Replaced: `musa_cub::`

- **dependencies/cub/cub/agent/agent_merge_sort.cuh:389**
  - Original: `cuda_cub::`
  - Replaced: `musa_cub::`

- **dependencies/cub/cub/agent/agent_merge_sort.cuh:390**
  - Original: `cuda_cub::`
  - Replaced: `musa_cub::`

- **dependencies/cub/cub/agent/agent_sub_warp_merge_sort.cuh:149**
  - Original: `cuda_cub::`
  - Replaced: `musa_cub::`

- **dependencies/cub/cub/agent/agent_sub_warp_merge_sort.cuh:151**
  - Original: `cuda_cub::`
  - Replaced: `musa_cub::`

  ... and 577 more

### 3. 命名空间 thrust::cuda

Count: 254

- **examples/musa/async_reduce.cu:27**
  - Original: `thrust::cuda::`
  - Replaced: `thrust::musa::`

- **examples/musa/custom_temporary_allocation.cu:17**
  - Original: `thrust::cuda::`
  - Replaced: `thrust::musa::`

- **examples/musa/custom_temporary_allocation.cu:84**
  - Original: `thrust::cuda::`
  - Replaced: `thrust::musa::`

- **examples/musa/custom_temporary_allocation.cu:90**
  - Original: `thrust::cuda::`
  - Replaced: `thrust::musa::`

- **examples/musa/custom_temporary_allocation.cu:92**
  - Original: `thrust::cuda::`
  - Replaced: `thrust::musa::`

- **examples/musa/custom_temporary_allocation.cu:141**
  - Original: `thrust::cuda::`
  - Replaced: `thrust::musa::`

- **examples/musa/custom_temporary_allocation.cu:141**
  - Original: `thrust::cuda::`
  - Replaced: `thrust::musa::`

- **examples/musa/custom_temporary_allocation.cu:149**
  - Original: `thrust::cuda::`
  - Replaced: `thrust::musa::`

- **examples/musa/custom_temporary_allocation.cu:149**
  - Original: `thrust::cuda::`
  - Replaced: `thrust::musa::`

- **examples/musa/custom_temporary_allocation.cu:163**
  - Original: `thrust::cuda::`
  - Replaced: `thrust::musa::`

  ... and 244 more

### 4. cuda_category 函数

Count: 17

- **examples/include/timer.h:32**
  - Original: `cuda_category(`
  - Replaced: `musa_category(`

- **internal/benchmark/bench.cu:774**
  - Original: `cuda_category(`
  - Replaced: `musa_category(`

- **internal/benchmark/bench.cu:818**
  - Original: `cuda_category(`
  - Replaced: `musa_category(`

- **internal/benchmark/bench.cu:859**
  - Original: `cuda_category(`
  - Replaced: `musa_category(`

- **internal/benchmark/bench.cu:896**
  - Original: `cuda_category(`
  - Replaced: `musa_category(`

- **internal/benchmark/bench.cu:936**
  - Original: `cuda_category(`
  - Replaced: `musa_category(`

- **thrust/system/musa/detail/error.inl:33**
  - Original: `cuda_category(`
  - Replaced: `musa_category(`

- **thrust/system/musa/detail/error.inl:39**
  - Original: `cuda_category(`
  - Replaced: `musa_category(`

- **thrust/system/musa/detail/error.inl:89**
  - Original: `cuda_category(`
  - Replaced: `musa_category(`

- **thrust/system/musa/detail/malloc_and_free.h:68**
  - Original: `cuda_category(`
  - Replaced: `musa_category(`

  ... and 7 more

### 5. cuda_error_category 类

Count: 4

- **thrust/system/musa/detail/error.inl:50**
  - Original: `cuda_error_category`
  - Replaced: `musa_error_category`

- **thrust/system/musa/detail/error.inl:54**
  - Original: `cuda_error_category`
  - Replaced: `musa_error_category`

- **thrust/system/musa/detail/error.inl:82**
  - Original: `cuda_error_category`
  - Replaced: `musa_error_category`

- **thrust/system/musa/detail/error.inl:91**
  - Original: `cuda_error_category`
  - Replaced: `musa_error_category`

### 6. 文档字符串 CUDA (Doxygen block)

Count: 300

- **dependencies/cub/benchmarks/musa_bench/musa_bench_helper.cuh:1**
  - Original: `/******************************************************************************
 * Copyright (c) 2011-2023, NVIDIA CORPORATION.  All rights reserved.
 * Copyright (c) 2024, Moore Threads Corporation.  All rights reserved.
 *
 * MUSA benchmark helper - replacement for nvbench_helper.cuh
 * Provides compatibility layer for porting CUDA benchmarks to MUSA.
 ******************************************************************************/`
  - Replaced: `/******************************************************************************
 * Copyright (c) 2011-2023, NVIDIA CORPORATION.  All rights reserved.
 * Copyright (c) 2024, Moore Threads Corporation.  All rights reserved.
 *
 * MUSA benchmark helper - replacement for nvbench_helper.cuh
 * Provides compatibility layer for porting MUSA benchmarks to MUSA.
 ******************************************************************************/`

- **dependencies/cub/cub/agent/agent_histogram.cuh:1**
  - Original: `/******************************************************************************
 * Copyright (c) 2011, Duane Merrill.  All rights reserved.
 * Copyright (c) 2011-2018, NVIDIA CORPORATION.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * \file
 * cub::AgentHistogram implements a stateful abstraction of CUDA thread blocks for participating in device-wide histogram .
 */`
  - Replaced: `/******************************************************************************
 * Copyright (c) 2011, Duane Merrill.  All rights reserved.
 * Copyright (c) 2011-2018, NVIDIA CORPORATION.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * \file
 * cub::AgentHistogram implements a stateful abstraction of MUSA thread blocks for participating in device-wide histogram .
 */`

- **dependencies/cub/cub/agent/agent_histogram.cuh:47**
  - Original: `/******************************************************************************
 * Tuning policy
 ******************************************************************************/

/**
 *
 */
enum BlockHistogramMemoryPreference
{
    GMEM,
    SMEM,
    BLEND
};


/**
 * Parameterizable tuning policy type for AgentHistogram
 */
template <
    int                             _BLOCK_THREADS,                 ///< Threads per thread block
    int                             _PIXELS_PER_THREAD,             ///< Pixels per thread (per tile of input)
    BlockLoadAlgorithm              _LOAD_ALGORITHM,                ///< The BlockLoad algorithm to use
    CacheLoadModifier               _LOAD_MODIFIER,                 ///< Cache load modifier for reading input elements
    bool                            _RLE_COMPRESS,                  ///< Whether to perform localized RLE to compress samples before histogramming
    BlockHistogramMemoryPreference  _MEM_PREFERENCE,                ///< Whether to prefer privatized shared-memory bins (versus privatized global-memory bins)
    bool                            _WORK_STEALING>                 ///< Whether to dequeue tiles from a global work queue
struct AgentHistogramPolicy
{
    enum
    {
        BLOCK_THREADS           = _BLOCK_THREADS,                   ///< Threads per thread block
        PIXELS_PER_THREAD       = _PIXELS_PER_THREAD,               ///< Pixels per thread (per tile of input)
        IS_RLE_COMPRESS         = _RLE_COMPRESS,                    ///< Whether to perform localized RLE to compress samples before histogramming
        MEM_PREFERENCE          = _MEM_PREFERENCE,                  ///< Whether to prefer privatized shared-memory bins (versus privatized global-memory bins)
        IS_WORK_STEALING        = _WORK_STEALING,                   ///< Whether to dequeue tiles from a global work queue
    };

    static const BlockLoadAlgorithm     LOAD_ALGORITHM          = _LOAD_ALGORITHM;          ///< The BlockLoad algorithm to use
    static const CacheLoadModifier      LOAD_MODIFIER           = _LOAD_MODIFIER;           ///< Cache load modifier for reading input elements
};


/******************************************************************************
 * Thread block abstractions
 ******************************************************************************/

/**
 * \brief AgentHistogram implements a stateful abstraction of CUDA thread blocks for participating in device-wide histogram .
 */`
  - Replaced: `/******************************************************************************
 * Tuning policy
 ******************************************************************************/

/**
 *
 */
enum BlockHistogramMemoryPreference
{
    GMEM,
    SMEM,
    BLEND
};


/**
 * Parameterizable tuning policy type for AgentHistogram
 */
template <
    int                             _BLOCK_THREADS,                 ///< Threads per thread block
    int                             _PIXELS_PER_THREAD,             ///< Pixels per thread (per tile of input)
    BlockLoadAlgorithm              _LOAD_ALGORITHM,                ///< The BlockLoad algorithm to use
    CacheLoadModifier               _LOAD_MODIFIER,                 ///< Cache load modifier for reading input elements
    bool                            _RLE_COMPRESS,                  ///< Whether to perform localized RLE to compress samples before histogramming
    BlockHistogramMemoryPreference  _MEM_PREFERENCE,                ///< Whether to prefer privatized shared-memory bins (versus privatized global-memory bins)
    bool                            _WORK_STEALING>                 ///< Whether to dequeue tiles from a global work queue
struct AgentHistogramPolicy
{
    enum
    {
        BLOCK_THREADS           = _BLOCK_THREADS,                   ///< Threads per thread block
        PIXELS_PER_THREAD       = _PIXELS_PER_THREAD,               ///< Pixels per thread (per tile of input)
        IS_RLE_COMPRESS         = _RLE_COMPRESS,                    ///< Whether to perform localized RLE to compress samples before histogramming
        MEM_PREFERENCE          = _MEM_PREFERENCE,                  ///< Whether to prefer privatized shared-memory bins (versus privatized global-memory bins)
        IS_WORK_STEALING        = _WORK_STEALING,                   ///< Whether to dequeue tiles from a global work queue
    };

    static const BlockLoadAlgorithm     LOAD_ALGORITHM          = _LOAD_ALGORITHM;          ///< The BlockLoad algorithm to use
    static const CacheLoadModifier      LOAD_MODIFIER           = _LOAD_MODIFIER;           ///< Cache load modifier for reading input elements
};


/******************************************************************************
 * Thread block abstractions
 ******************************************************************************/

/**
 * \brief AgentHistogram implements a stateful abstraction of MUSA thread blocks for participating in device-wide histogram .
 */`

- **dependencies/cub/cub/agent/agent_radix_sort_downsweep.cuh:1**
  - Original: `/******************************************************************************
 * Copyright (c) 2011, Duane Merrill.  All rights reserved.
 * Copyright (c) 2011-2018, NVIDIA CORPORATION.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * \file
 * AgentRadixSortDownsweep implements a stateful abstraction of CUDA thread
 * blocks for participating in device-wide radix sort downsweep .
 */`
  - Replaced: `/******************************************************************************
 * Copyright (c) 2011, Duane Merrill.  All rights reserved.
 * Copyright (c) 2011-2018, NVIDIA CORPORATION.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * \file
 * AgentRadixSortDownsweep implements a stateful abstraction of MUSA thread
 * blocks for participating in device-wide radix sort downsweep .
 */`

- **dependencies/cub/cub/agent/agent_radix_sort_downsweep.cuh:54**
  - Original: `/******************************************************************************
 * Tuning policy types
 ******************************************************************************/

/**
 * Parameterizable tuning policy type for AgentRadixSortDownsweep
 */
template <
    int                 NOMINAL_BLOCK_THREADS_4B,       ///< Threads per thread block
    int                 NOMINAL_ITEMS_PER_THREAD_4B,    ///< Items per thread (per tile of input)
    typename            ComputeT,                       ///< Dominant compute type
    BlockLoadAlgorithm  _LOAD_ALGORITHM,                ///< The BlockLoad algorithm to use
    CacheLoadModifier   _LOAD_MODIFIER,                 ///< Cache load modifier for reading keys (and values)
    RadixRankAlgorithm  _RANK_ALGORITHM,                ///< The radix ranking algorithm to use
    BlockScanAlgorithm  _SCAN_ALGORITHM,                ///< The block scan algorithm to use
    int                 _RADIX_BITS,                    ///< The number of radix bits, i.e., log2(bins)
    typename            ScalingType = RegBoundScaling<NOMINAL_BLOCK_THREADS_4B, NOMINAL_ITEMS_PER_THREAD_4B, ComputeT> >
struct AgentRadixSortDownsweepPolicy :
    ScalingType
{
    enum
    {
        RADIX_BITS              = _RADIX_BITS,              ///< The number of radix bits, i.e., log2(bins)
    };

    static const BlockLoadAlgorithm  LOAD_ALGORITHM     = _LOAD_ALGORITHM;    ///< The BlockLoad algorithm to use
    static const CacheLoadModifier   LOAD_MODIFIER      = _LOAD_MODIFIER;     ///< Cache load modifier for reading keys (and values)
    static const RadixRankAlgorithm  RANK_ALGORITHM     = _RANK_ALGORITHM;    ///< The radix ranking algorithm to use
    static const BlockScanAlgorithm  SCAN_ALGORITHM     = _SCAN_ALGORITHM;    ///< The BlockScan algorithm to use
};


/******************************************************************************
 * Thread block abstractions
 ******************************************************************************/





/**
 * \brief AgentRadixSortDownsweep implements a stateful abstraction of CUDA thread blocks for participating in device-wide radix sort downsweep .
 */`
  - Replaced: `/******************************************************************************
 * Tuning policy types
 ******************************************************************************/

/**
 * Parameterizable tuning policy type for AgentRadixSortDownsweep
 */
template <
    int                 NOMINAL_BLOCK_THREADS_4B,       ///< Threads per thread block
    int                 NOMINAL_ITEMS_PER_THREAD_4B,    ///< Items per thread (per tile of input)
    typename            ComputeT,                       ///< Dominant compute type
    BlockLoadAlgorithm  _LOAD_ALGORITHM,                ///< The BlockLoad algorithm to use
    CacheLoadModifier   _LOAD_MODIFIER,                 ///< Cache load modifier for reading keys (and values)
    RadixRankAlgorithm  _RANK_ALGORITHM,                ///< The radix ranking algorithm to use
    BlockScanAlgorithm  _SCAN_ALGORITHM,                ///< The block scan algorithm to use
    int                 _RADIX_BITS,                    ///< The number of radix bits, i.e., log2(bins)
    typename            ScalingType = RegBoundScaling<NOMINAL_BLOCK_THREADS_4B, NOMINAL_ITEMS_PER_THREAD_4B, ComputeT> >
struct AgentRadixSortDownsweepPolicy :
    ScalingType
{
    enum
    {
        RADIX_BITS              = _RADIX_BITS,              ///< The number of radix bits, i.e., log2(bins)
    };

    static const BlockLoadAlgorithm  LOAD_ALGORITHM     = _LOAD_ALGORITHM;    ///< The BlockLoad algorithm to use
    static const CacheLoadModifier   LOAD_MODIFIER      = _LOAD_MODIFIER;     ///< Cache load modifier for reading keys (and values)
    static const RadixRankAlgorithm  RANK_ALGORITHM     = _RANK_ALGORITHM;    ///< The radix ranking algorithm to use
    static const BlockScanAlgorithm  SCAN_ALGORITHM     = _SCAN_ALGORITHM;    ///< The BlockScan algorithm to use
};


/******************************************************************************
 * Thread block abstractions
 ******************************************************************************/





/**
 * \brief AgentRadixSortDownsweep implements a stateful abstraction of MUSA thread blocks for participating in device-wide radix sort downsweep .
 */`

- **dependencies/cub/cub/agent/agent_radix_sort_histogram.cuh:1**
  - Original: `/******************************************************************************
 * Copyright (c) 2011-2020, NVIDIA CORPORATION.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * \file
 * agent_radix_sort_histogram.cuh implements a stateful abstraction of CUDA
 * thread blocks for participating in the device histogram kernel used for
 * one-sweep radix sorting.
 */`
  - Replaced: `/******************************************************************************
 * Copyright (c) 2011-2020, NVIDIA CORPORATION.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * \file
 * agent_radix_sort_histogram.cuh implements a stateful abstraction of MUSA
 * thread blocks for participating in the device histogram kernel used for
 * one-sweep radix sorting.
 */`

- **dependencies/cub/cub/agent/agent_radix_sort_onesweep.cuh:1**
  - Original: `/******************************************************************************
 * Copyright (c) 2011-2020, NVIDIA CORPORATION.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * \file
 * agent_radix_sort_onesweep.cuh implements a stateful abstraction of CUDA
 * thread blocks for participating in the device one-sweep radix sort kernel.
 */`
  - Replaced: `/******************************************************************************
 * Copyright (c) 2011-2020, NVIDIA CORPORATION.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * \file
 * agent_radix_sort_onesweep.cuh implements a stateful abstraction of MUSA
 * thread blocks for participating in the device one-sweep radix sort kernel.
 */`

- **dependencies/cub/cub/agent/agent_radix_sort_upsweep.cuh:1**
  - Original: `/******************************************************************************
 * Copyright (c) 2011, Duane Merrill.  All rights reserved.
 * Copyright (c) 2011-2018, NVIDIA CORPORATION.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * \file
 * AgentRadixSortUpsweep implements a stateful abstraction of CUDA thread blocks for participating in device-wide radix sort upsweep .
 */`
  - Replaced: `/******************************************************************************
 * Copyright (c) 2011, Duane Merrill.  All rights reserved.
 * Copyright (c) 2011-2018, NVIDIA CORPORATION.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * \file
 * AgentRadixSortUpsweep implements a stateful abstraction of MUSA thread blocks for participating in device-wide radix sort upsweep .
 */`

- **dependencies/cub/cub/agent/agent_radix_sort_upsweep.cuh:47**
  - Original: `/******************************************************************************
 * Tuning policy types
 ******************************************************************************/

/**
 * Parameterizable tuning policy type for AgentRadixSortUpsweep
 */
template <
    int                 NOMINAL_BLOCK_THREADS_4B,       ///< Threads per thread block
    int                 NOMINAL_ITEMS_PER_THREAD_4B,    ///< Items per thread (per tile of input)
    typename            ComputeT,                       ///< Dominant compute type
    CacheLoadModifier   _LOAD_MODIFIER,                 ///< Cache load modifier for reading keys
    int                 _RADIX_BITS,                    ///< The number of radix bits, i.e., log2(bins)
    typename            ScalingType = RegBoundScaling<NOMINAL_BLOCK_THREADS_4B, NOMINAL_ITEMS_PER_THREAD_4B, ComputeT> >
struct AgentRadixSortUpsweepPolicy :
    ScalingType
{
    enum
    {
        RADIX_BITS          = _RADIX_BITS,          ///< The number of radix bits, i.e., log2(bins)
    };

    static const CacheLoadModifier LOAD_MODIFIER = _LOAD_MODIFIER;      ///< Cache load modifier for reading keys
};


/******************************************************************************
 * Thread block abstractions
 ******************************************************************************/

/**
 * \brief AgentRadixSortUpsweep implements a stateful abstraction of CUDA thread blocks for participating in device-wide radix sort upsweep .
 */`
  - Replaced: `/******************************************************************************
 * Tuning policy types
 ******************************************************************************/

/**
 * Parameterizable tuning policy type for AgentRadixSortUpsweep
 */
template <
    int                 NOMINAL_BLOCK_THREADS_4B,       ///< Threads per thread block
    int                 NOMINAL_ITEMS_PER_THREAD_4B,    ///< Items per thread (per tile of input)
    typename            ComputeT,                       ///< Dominant compute type
    CacheLoadModifier   _LOAD_MODIFIER,                 ///< Cache load modifier for reading keys
    int                 _RADIX_BITS,                    ///< The number of radix bits, i.e., log2(bins)
    typename            ScalingType = RegBoundScaling<NOMINAL_BLOCK_THREADS_4B, NOMINAL_ITEMS_PER_THREAD_4B, ComputeT> >
struct AgentRadixSortUpsweepPolicy :
    ScalingType
{
    enum
    {
        RADIX_BITS          = _RADIX_BITS,          ///< The number of radix bits, i.e., log2(bins)
    };

    static const CacheLoadModifier LOAD_MODIFIER = _LOAD_MODIFIER;      ///< Cache load modifier for reading keys
};


/******************************************************************************
 * Thread block abstractions
 ******************************************************************************/

/**
 * \brief AgentRadixSortUpsweep implements a stateful abstraction of MUSA thread blocks for participating in device-wide radix sort upsweep .
 */`

- **dependencies/cub/cub/agent/agent_reduce.cuh:1**
  - Original: `/******************************************************************************
 * Copyright (c) 2011, Duane Merrill.  All rights reserved.
 * Copyright (c) 2011-2018, NVIDIA CORPORATION.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * \file
 * cub::AgentReduce implements a stateful abstraction of CUDA thread blocks for participating in device-wide reduction .
 */`
  - Replaced: `/******************************************************************************
 * Copyright (c) 2011, Duane Merrill.  All rights reserved.
 * Copyright (c) 2011-2018, NVIDIA CORPORATION.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/**
 * \file
 * cub::AgentReduce implements a stateful abstraction of MUSA thread blocks for participating in device-wide reduction .
 */`

  ... and 290 more

### 6. 文档字符串 CUDA (Single line comment)

Count: 128

- **dependencies/cub/cub/block/block_store.cuh:146**
  - Original: `// Maximum CUDA`
  - Replaced: `// Maximum MUSA`

- **dependencies/cub/cub/device/device_histogram.cuh:230**
  - Original: `///< [in] <b>[optional]</b> CUDA`
  - Replaced: `///< [in] <b>[optional]</b> MUSA`

- **dependencies/cub/cub/device/device_histogram.cuh:439**
  - Original: `///< [in] <b>[optional]</b> CUDA`
  - Replaced: `///< [in] <b>[optional]</b> MUSA`

- **dependencies/cub/cub/device/device_histogram.cuh:629**
  - Original: `///< [in] <b>[optional]</b> CUDA`
  - Replaced: `///< [in] <b>[optional]</b> MUSA`

- **dependencies/cub/cub/device/device_histogram.cuh:831**
  - Original: `///< [in] <b>[optional]</b> CUDA`
  - Replaced: `///< [in] <b>[optional]</b> MUSA`

- **dependencies/cub/cub/device/device_radix_sort.cuh:198**
  - Original: `///< [in] <b>[optional]</b> CUDA`
  - Replaced: `///< [in] <b>[optional]</b> MUSA`

- **dependencies/cub/cub/device/device_radix_sort.cuh:310**
  - Original: `///< [in] <b>[optional]</b> CUDA`
  - Replaced: `///< [in] <b>[optional]</b> MUSA`

- **dependencies/cub/cub/device/device_radix_sort.cuh:511**
  - Original: `///< [in] <b>[optional]</b> CUDA`
  - Replaced: `///< [in] <b>[optional]</b> MUSA`

- **dependencies/cub/cub/device/device_radix_sort.cuh:604**
  - Original: `///< [in] <b>[optional]</b> CUDA`
  - Replaced: `///< [in] <b>[optional]</b> MUSA`

- **dependencies/cub/cub/device/device_radix_sort.cuh:703**
  - Original: `///< [in] <b>[optional]</b> CUDA`
  - Replaced: `///< [in] <b>[optional]</b> MUSA`

  ... and 118 more
