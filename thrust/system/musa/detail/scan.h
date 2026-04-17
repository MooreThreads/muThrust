/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
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

#include <thrust/detail/config.h>

#if THRUST_DEVICE_COMPILER == THRUST_DEVICE_COMPILER_NVCC

#include <thrust/detail/config/exec_check_disable.h>
#include <thrust/detail/cstdint.h>
#include <thrust/detail/type_traits.h>
#include <thrust/distance.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/type_traits/is_contiguous_iterator.h>
#include <thrust/system/musa/config.h>
#include <thrust/system/musa/detail/dispatch.h>
#include <thrust/system/musa/detail/pair_vector_adapter.h>

#include <cub/device/device_scan.cuh>

THRUST_NAMESPACE_BEGIN
namespace musa_cub
{
namespace detail
{

__thrust_exec_check_disable__
template <typename Derived,
          typename InputIt,
          typename Size,
          typename OutputIt,
          typename ScanOp>
__host__ __device__
OutputIt inclusive_scan_n_impl(thrust::musa_cub::execution_policy<Derived> &policy,
                               InputIt first,
                               Size num_items,
                               OutputIt result,
                               ScanOp scan_op)
{
  using PairValueT = typename thrust::iterator_value<InputIt>::type;
  using OutputValueT = typename thrust::iterator_value<OutputIt>::type;
  using UnwrapInputIt =
    thrust::detail::try_unwrap_contiguous_iterator_return_t<InputIt>;
  using UnwrapOutputIt =
    thrust::detail::try_unwrap_contiguous_iterator_return_t<OutputIt>;

  auto first_unwrap = thrust::detail::try_unwrap_contiguous_iterator(first);
  auto result_unwrap = thrust::detail::try_unwrap_contiguous_iterator(result);

  if constexpr (thrust::musa_cub::detail::is_small_pair_vectorizable<PairValueT>::value
                && std::is_same<PairValueT, OutputValueT>::value
                && std::is_pointer<UnwrapInputIt>::value
                && std::is_pointer<UnwrapOutputIt>::value)
  {
    using Adapter = thrust::musa_cub::detail::pair_vector_adapter<PairValueT>;
    using StorageT = typename Adapter::storage_type;
    using StorageOp =
      thrust::musa_cub::detail::pair_vector_scan_op<ScanOp, PairValueT>;
    using Dispatch32 = cub::DispatchScan<const StorageT*,
                                         StorageT*,
                                         StorageOp,
                                         cub::NullType,
                                         thrust::detail::int32_t>;
    using Dispatch64 = cub::DispatchScan<const StorageT*,
                                         StorageT*,
                                         StorageOp,
                                         cub::NullType,
                                         thrust::detail::int64_t>;

    const StorageT* storage_first =
      thrust::musa_cub::detail::pair_storage_pointer(first_unwrap);
    StorageT* storage_result =
      thrust::musa_cub::detail::pair_storage_pointer(result_unwrap);
    StorageOp storage_scan_op{scan_op};
    musaStream_t stream = thrust::musa_cub::stream(policy);
    musaError_t status;

    size_t tmp_size = 0;
    THRUST_INDEX_TYPE_DISPATCH2(status,
                                Dispatch32::Dispatch,
                                Dispatch64::Dispatch,
                                num_items,
                                (nullptr,
                                 tmp_size,
                                 storage_first,
                                 storage_result,
                                 storage_scan_op,
                                 cub::NullType{},
                                 num_items_fixed,
                                 stream,
                                 THRUST_DEBUG_SYNC_FLAG));
    thrust::musa_cub::throw_on_error(status,
                                     "after determining tmp storage "
                                     "requirements for pair inclusive_scan");

    thrust::detail::temporary_array<thrust::detail::uint8_t, Derived> tmp{
      policy,
      tmp_size};
    THRUST_INDEX_TYPE_DISPATCH2(status,
                                Dispatch32::Dispatch,
                                Dispatch64::Dispatch,
                                num_items,
                                (tmp.data().get(),
                                 tmp_size,
                                 storage_first,
                                 storage_result,
                                 storage_scan_op,
                                 cub::NullType{},
                                 num_items_fixed,
                                 stream,
                                 THRUST_DEBUG_SYNC_FLAG));
    thrust::musa_cub::throw_on_error(
      status, "after dispatching pair inclusive_scan kernel");
    thrust::musa_cub::throw_on_error(thrust::musa_cub::synchronize_optional(policy),
                                     "pair inclusive_scan failed to synchronize");
    return result + num_items;
  }

  using Dispatch32 = cub::DispatchScan<InputIt,
                                       OutputIt,
                                       ScanOp,
                                       cub::NullType,
                                       thrust::detail::int32_t>;
  using Dispatch64 = cub::DispatchScan<InputIt,
                                       OutputIt,
                                       ScanOp,
                                       cub::NullType,
                                       thrust::detail::int64_t>;

  musaStream_t stream = thrust::musa_cub::stream(policy);
  musaError_t status;

  // Determine temporary storage requirements:
  size_t tmp_size = 0;
  {
    THRUST_INDEX_TYPE_DISPATCH2(status,
                                Dispatch32::Dispatch,
                                Dispatch64::Dispatch,
                                num_items,
                                (nullptr,
                                 tmp_size,
                                 first,
                                 result,
                                 scan_op,
                                 cub::NullType{},
                                 num_items_fixed,
                                 stream,
                                 THRUST_DEBUG_SYNC_FLAG));
    thrust::musa_cub::throw_on_error(status,
                                     "after determining tmp storage "
                                     "requirements for inclusive_scan");
  }

  // Run scan:
  {
    // Allocate temporary storage:
    thrust::detail::temporary_array<thrust::detail::uint8_t, Derived> tmp{
      policy,
      tmp_size};
    THRUST_INDEX_TYPE_DISPATCH2(status,
                                Dispatch32::Dispatch,
                                Dispatch64::Dispatch,
                                num_items,
                                (tmp.data().get(),
                                 tmp_size,
                                 first,
                                 result,
                                 scan_op,
                                 cub::NullType{},
                                 num_items_fixed,
                                 stream,
                                 THRUST_DEBUG_SYNC_FLAG));
    thrust::musa_cub::throw_on_error(status,
                                     "after dispatching inclusive_scan kernel");
    thrust::musa_cub::throw_on_error(thrust::musa_cub::synchronize_optional(policy),
                                     "inclusive_scan failed to synchronize");
  }

  return result + num_items;
}

__thrust_exec_check_disable__
template <typename Derived,
          typename InputIt,
          typename Size,
          typename OutputIt,
          typename InitValueT,
          typename ScanOp>
__host__ __device__
OutputIt exclusive_scan_n_impl(thrust::musa_cub::execution_policy<Derived> &policy,
                               InputIt first,
                               Size num_items,
                               OutputIt result,
                               InitValueT init,
                               ScanOp scan_op)
{
  using PairValueT = typename thrust::iterator_value<InputIt>::type;
  using OutputValueT = typename thrust::iterator_value<OutputIt>::type;
  using UnwrapInputIt =
    thrust::detail::try_unwrap_contiguous_iterator_return_t<InputIt>;
  using UnwrapOutputIt =
    thrust::detail::try_unwrap_contiguous_iterator_return_t<OutputIt>;

  auto first_unwrap = thrust::detail::try_unwrap_contiguous_iterator(first);
  auto result_unwrap = thrust::detail::try_unwrap_contiguous_iterator(result);

  if constexpr (thrust::musa_cub::detail::is_small_pair_vectorizable<PairValueT>::value
                && std::is_same<PairValueT, OutputValueT>::value
                && std::is_pointer<UnwrapInputIt>::value
                && std::is_pointer<UnwrapOutputIt>::value)
  {
    using Adapter = thrust::musa_cub::detail::pair_vector_adapter<PairValueT>;
    using StorageT = typename Adapter::storage_type;
    using StorageInitT = cub::detail::InputValue<StorageT>;
    using StorageOp =
      thrust::musa_cub::detail::pair_vector_scan_op<ScanOp, PairValueT>;
    using Dispatch32 = cub::DispatchScan<const StorageT*,
                                         StorageT*,
                                         StorageOp,
                                         StorageInitT,
                                         thrust::detail::int32_t>;
    using Dispatch64 = cub::DispatchScan<const StorageT*,
                                         StorageT*,
                                         StorageOp,
                                         StorageInitT,
                                         thrust::detail::int64_t>;

    const StorageT* storage_first =
      thrust::musa_cub::detail::pair_storage_pointer(first_unwrap);
    StorageT* storage_result =
      thrust::musa_cub::detail::pair_storage_pointer(result_unwrap);
    const StorageInitT storage_init{Adapter::to_storage(init)};
    StorageOp storage_scan_op{scan_op};
    musaStream_t stream = thrust::musa_cub::stream(policy);
    musaError_t status;

    size_t tmp_size = 0;
    THRUST_INDEX_TYPE_DISPATCH2(status,
                                Dispatch32::Dispatch,
                                Dispatch64::Dispatch,
                                num_items,
                                (nullptr,
                                 tmp_size,
                                 storage_first,
                                 storage_result,
                                 storage_scan_op,
                                 storage_init,
                                 num_items_fixed,
                                 stream,
                                 THRUST_DEBUG_SYNC_FLAG));
    thrust::musa_cub::throw_on_error(status,
                                     "after determining tmp storage "
                                     "requirements for pair exclusive_scan");

    thrust::detail::temporary_array<thrust::detail::uint8_t, Derived> tmp{
      policy,
      tmp_size};
    THRUST_INDEX_TYPE_DISPATCH2(status,
                                Dispatch32::Dispatch,
                                Dispatch64::Dispatch,
                                num_items,
                                (tmp.data().get(),
                                 tmp_size,
                                 storage_first,
                                 storage_result,
                                 storage_scan_op,
                                 storage_init,
                                 num_items_fixed,
                                 stream,
                                 THRUST_DEBUG_SYNC_FLAG));
    thrust::musa_cub::throw_on_error(
      status, "after dispatching pair exclusive_scan kernel");
    thrust::musa_cub::throw_on_error(thrust::musa_cub::synchronize_optional(policy),
                                     "pair exclusive_scan failed to synchronize");
    return result + num_items;
  }

  using InputValueT = cub::detail::InputValue<InitValueT>;
  using Dispatch32 = cub::DispatchScan<InputIt,
                                       OutputIt,
                                       ScanOp,
                                       InputValueT,
                                       thrust::detail::int32_t>;
  using Dispatch64 = cub::DispatchScan<InputIt,
                                       OutputIt,
                                       ScanOp,
                                       InputValueT,
                                       thrust::detail::int64_t>;

  musaStream_t stream = thrust::musa_cub::stream(policy);
  musaError_t status;

  // Determine temporary storage requirements:
  size_t tmp_size = 0;
  {
    THRUST_INDEX_TYPE_DISPATCH2(status,
                                Dispatch32::Dispatch,
                                Dispatch64::Dispatch,
                                num_items,
                                (nullptr,
                                 tmp_size,
                                 first,
                                 result,
                                 scan_op,
                                 InputValueT(init),
                                 num_items_fixed,
                                 stream,
                                 THRUST_DEBUG_SYNC_FLAG));
    thrust::musa_cub::throw_on_error(status,
                                     "after determining tmp storage "
                                     "requirements for exclusive_scan");
  }

  // Run scan:
  {
    // Allocate temporary storage:
    thrust::detail::temporary_array<thrust::detail::uint8_t, Derived> tmp{
      policy,
      tmp_size};
    THRUST_INDEX_TYPE_DISPATCH2(status,
                                Dispatch32::Dispatch,
                                Dispatch64::Dispatch,
                                num_items,
                                (tmp.data().get(),
                                 tmp_size,
                                 first,
                                 result,
                                 scan_op,
                                 InputValueT(init),
                                 num_items_fixed,
                                 stream,
                                 THRUST_DEBUG_SYNC_FLAG));
    thrust::musa_cub::throw_on_error(status,
                                     "after dispatching exclusive_scan kernel");
    thrust::musa_cub::throw_on_error(thrust::musa_cub::synchronize_optional(policy),
                                     "exclusive_scan failed to synchronize");
  }

  return result + num_items;
}

} // namespace detail

//-------------------------
// Thrust API entry points
//-------------------------

__thrust_exec_check_disable__
template <typename Derived,
          typename InputIt,
          typename Size,
          typename OutputIt,
          typename ScanOp>
__host__ __device__
OutputIt inclusive_scan_n(thrust::musa_cub::execution_policy<Derived> &policy,
                          InputIt first,
                          Size num_items,
                          OutputIt result,
                          ScanOp scan_op)
{
  OutputIt ret = result;
  if (__THRUST_HAS_CUDART__)
  {
    ret = thrust::musa_cub::detail::inclusive_scan_n_impl(policy,
                                                          first,
                                                          num_items,
                                                          result,
                                                          scan_op);
  }
  else
  {
#if !__THRUST_HAS_CUDART__
    ret = thrust::inclusive_scan(cvt_to_seq(derived_cast(policy)),
                                 first,
                                 first + num_items,
                                 result,
                                 scan_op);
#endif
  }
  return ret;
}

template <typename Derived, typename InputIt, typename OutputIt, typename ScanOp>
__host__ __device__
OutputIt inclusive_scan(thrust::musa_cub::execution_policy<Derived> &policy,
                        InputIt first,
                        InputIt last,
                        OutputIt result,
                        ScanOp scan_op)
{
  using diff_t = typename thrust::iterator_traits<InputIt>::difference_type;
  diff_t const num_items = thrust::distance(first, last);
  return thrust::musa_cub::inclusive_scan_n(policy,
                                            first,
                                            num_items,
                                            result,
                                            scan_op);
}

template <typename Derived, typename InputIt, typename OutputIt>
__host__ __device__
OutputIt inclusive_scan(thrust::musa_cub::execution_policy<Derived> &policy,
                        InputIt first,
                        InputIt last,
                        OutputIt result)
{
  return thrust::musa_cub::inclusive_scan(policy,
                                          first,
                                          last,
                                          result,
                                          thrust::plus<>{});
}

__thrust_exec_check_disable__
template <typename Derived,
          typename InputIt,
          typename Size,
          typename OutputIt,
          typename T,
          typename ScanOp>
__host__ __device__
OutputIt exclusive_scan_n(thrust::musa_cub::execution_policy<Derived> &policy,
                          InputIt first,
                          Size num_items,
                          OutputIt result,
                          T init,
                          ScanOp scan_op)
{
  OutputIt ret = result;
  if (__THRUST_HAS_CUDART__)
  {
    ret = thrust::musa_cub::detail::exclusive_scan_n_impl(policy,
                                                          first,
                                                          num_items,
                                                          result,
                                                          init,
                                                          scan_op);
  }
  else
  {
#if !__THRUST_HAS_CUDART__
    ret = thrust::exclusive_scan(cvt_to_seq(derived_cast(policy)),
                                 first,
                                 first + num_items,
                                 result,
                                 init,
                                 scan_op);
#endif
  }
  return ret;
}

template <typename Derived,
          typename InputIt,
          typename OutputIt,
          typename T,
          typename ScanOp>
__host__ __device__
OutputIt exclusive_scan(thrust::musa_cub::execution_policy<Derived> &policy,
                        InputIt first,
                        InputIt last,
                        OutputIt result,
                        T init,
                        ScanOp scan_op)
{
  using diff_t = typename thrust::iterator_traits<InputIt>::difference_type;
  diff_t const num_items = thrust::distance(first, last);
  return thrust::musa_cub::exclusive_scan_n(policy,
                                            first,
                                            num_items,
                                            result,
                                            init,
                                            scan_op);
}

template <typename Derived, typename InputIt, typename OutputIt, typename T>
__host__ __device__
OutputIt exclusive_scan(thrust::musa_cub::execution_policy<Derived> &policy,
                        InputIt first,
                        InputIt last,
                        OutputIt result,
                        T init)
{
  return thrust::musa_cub::exclusive_scan(policy,
                                          first,
                                          last,
                                          result,
                                          init,
                                          thrust::plus<>{});
}

template <typename Derived, typename InputIt, typename OutputIt>
__host__ __device__
OutputIt exclusive_scan(thrust::musa_cub::execution_policy<Derived> &policy,
                        InputIt first,
                        InputIt last,
                        OutputIt result)
{
  using init_type = typename thrust::iterator_traits<InputIt>::value_type;
  return musa_cub::exclusive_scan(policy, first, last, result, init_type{});
};

} // namespace musa_cub
THRUST_NAMESPACE_END

#include <thrust/scan.h>

#endif // NVCC
