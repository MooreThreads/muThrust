/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
/*
 *  Copyright 2008-2013 NVIDIA Corporation
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

/*! \file thrust/system/musa/vector.h
 *  \brief A dynamically-sizable array of elements which reside in memory available to
 *         Thrust's CUDA system.
 */

#pragma once

#include <thrust/detail/config.h>
#include <thrust/system/musa/memory.h>
#include <thrust/detail/vector_base.h>
#include <vector>

namespace thrust { namespace cuda_cub
{

/*! \p musa::vector is a container that supports random access to elements,
 *  constant time removal of elements at the end, and linear time insertion
 *  and removal of elements at the beginning or in the middle. The number of
 *  elements in a \p musa::vector may vary dynamically; memory management is
 *  automatic. The elements contained in a \p musa::vector reside in memory
 *  accessible by the \p musa system.
 *
 *  \tparam T The element type of the \p musa::vector.
 *  \tparam Allocator The allocator type of the \p musa::vector.
 *          Defaults to \p musa::allocator.
 *
 *  \see https://en.cppreference.com/w/cpp/container/vector
 *  \see host_vector For the documentation of the complete interface which is
 *                   shared by \p musa::vector
 *  \see device_vector
 *  \see universal_vector
 */
template <typename T, typename Allocator = thrust::system::musa::allocator<T>>
using vector = thrust::detail::vector_base<T, Allocator>;

/*! \p musa::universal_vector is a container that supports random access to
 *  elements, constant time removal of elements at the end, and linear time
 *  insertion and removal of elements at the beginning or in the middle. The
 *  number of elements in a \p musa::universal_vector may vary dynamically;
 *  memory management is automatic. The elements contained in a
 *  \p musa::universal_vector reside in memory accessible by the \p musa system
 *  and host systems.
 *
 *  \tparam T The element type of the \p musa::universal_vector.
 *  \tparam Allocator The allocator type of the \p musa::universal_vector.
 *          Defaults to \p musa::universal_allocator.
 *
 *  \see https://en.cppreference.com/w/cpp/container/vector
 *  \see host_vector For the documentation of the complete interface which is
 *                   shared by \p musa::universal_vector
 *  \see device_vector
 *  \see universal_vector
 */
template <typename T, typename Allocator = thrust::system::musa::universal_allocator<T>>
using universal_vector = thrust::detail::vector_base<T, Allocator>;

} // namespace cuda_cub

namespace system { namespace musa
{
using thrust::cuda_cub::vector;
using thrust::cuda_cub::universal_vector;
}}

namespace musa
{
using thrust::cuda_cub::vector;
using thrust::cuda_cub::universal_vector;
}

} // namespace thrust

