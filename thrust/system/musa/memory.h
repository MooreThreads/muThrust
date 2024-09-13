/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
/*
 *  Copyright 2008-2018 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*! \file thrust/system/musa/memory.h
 *  \brief Managing memory associated with Thrust's CUDA system.
 */

#pragma once

#include <thrust/detail/config.h>
#include <thrust/system/musa/memory_resource.h>
#include <thrust/memory.h>
#include <thrust/detail/type_traits.h>
#include <thrust/mr/allocator.h>
#include <ostream>

namespace thrust { namespace cuda_cub
{

/*! Allocates an area of memory available to Thrust's <tt>musa</tt> system.
 *  \param n Number of bytes to allocate.
 *  \return A <tt>musa::pointer<void></tt> pointing to the beginning of the newly
 *          allocated memory. A null <tt>musa::pointer<void></tt> is returned if
 *          an error occurs.
 *  \note The <tt>musa::pointer<void></tt> returned by this function must be
 *        deallocated with \p musa::free.
 *  \see musa::free
 *  \see std::malloc
 */
inline __host__ __device__ pointer<void> malloc(std::size_t n);

/*! Allocates a typed area of memory available to Thrust's <tt>musa</tt> system.
 *  \param n Number of elements to allocate.
 *  \return A <tt>musa::pointer<T></tt> pointing to the beginning of the newly
 *          allocated elements. A null <tt>musa::pointer<T></tt> is returned if
 *          an error occurs.
 *  \note The <tt>musa::pointer<T></tt> returned by this function must be
 *        deallocated with \p musa::free.
 *  \see musa::free
 *  \see std::malloc
 */
template <typename T>
inline __host__ __device__ pointer<T> malloc(std::size_t n);

/*! Deallocates an area of memory previously allocated by <tt>musa::malloc</tt>.
 *  \param ptr A <tt>musa::pointer<void></tt> pointing to the beginning of an area
 *         of memory previously allocated with <tt>musa::malloc</tt>.
 *  \see musa::malloc
 *  \see std::free
 */
inline __host__ __device__ void free(pointer<void> ptr);

/*! \p musa::allocator is the default allocator used by the \p musa system's
 *  containers such as <tt>musa::vector</tt> if no user-specified allocator is
 *  provided. \p musa::allocator allocates (deallocates) storage with \p
 *  musa::malloc (\p musa::free).
 */
template<typename T>
using allocator = thrust::mr::stateless_resource_allocator<
  T, thrust::system::musa::memory_resource
>;

/*! \p musa::universal_allocator allocates memory that can be used by the \p musa
 *  system and host systems.
 */
template<typename T>
using universal_allocator = thrust::mr::stateless_resource_allocator<
  T, thrust::system::musa::universal_memory_resource
>;

} // namespace cuda_cub

namespace system { namespace musa
{
using thrust::cuda_cub::malloc;
using thrust::cuda_cub::free;
using thrust::cuda_cub::allocator;
using thrust::cuda_cub::universal_allocator;
}} // namespace system::musa

/*! \namespace thrust::musa
 *  \brief \p thrust::musa is a top-level alias for \p thrust::system::musa.
 */
namespace musa
{
using thrust::cuda_cub::malloc;
using thrust::cuda_cub::free;
using thrust::cuda_cub::allocator;
using thrust::cuda_cub::universal_allocator;
} // namespace musa

} // namespace thrust

#include <thrust/system/musa/detail/memory.inl>

