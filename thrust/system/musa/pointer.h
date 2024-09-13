/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
/*
 *  Copyright 2008-2020 NVIDIA Corporation
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
 *  \brief Managing memory associated with Thrust's Standard C++ system.
 */

#pragma once

#include <thrust/detail/config.h>
#include <type_traits>
#include <thrust/system/musa/detail/execution_policy.h>
#include <thrust/detail/pointer.h>
#include <thrust/detail/reference.h>

namespace thrust { namespace cuda_cub
{

/*! \p musa::pointer stores a pointer to an object allocated in memory
 *  accessible by the \p musa system. This type provides type safety when
 *  dispatching algorithms on ranges resident in \p musa memory.
 *
 *  \p musa::pointer has pointer semantics: it may be dereferenced and
 *  manipulated with pointer arithmetic.
 *
 *  \p musa::pointer can be created with the function \p musa::malloc, or by
 *  explicitly calling its constructor with a raw pointer.
 *
 *  The raw pointer encapsulated by a \p musa::pointer may be obtained by eiter
 *  its <tt>get</tt> member function or the \p raw_pointer_cast function.
 *
 *  \note \p musa::pointer is not a "smart" pointer; it is the programmer's
 *        responsibility to deallocate memory pointed to by \p musa::pointer.
 *
 *  \tparam T specifies the type of the pointee.
 *
 *  \see musa::malloc
 *  \see musa::free
 *  \see raw_pointer_cast
 */
template <typename T>
using pointer = thrust::pointer<
  T,
  thrust::cuda_cub::tag,
  thrust::tagged_reference<T, thrust::cuda_cub::tag>
>;

/*! \p musa::universal_pointer stores a pointer to an object allocated in
 *  memory accessible by the \p musa system and host systems.
 *
 *  \p musa::universal_pointer has pointer semantics: it may be dereferenced
 *  and manipulated with pointer arithmetic.
 *
 *  \p musa::universal_pointer can be created with \p musa::universal_allocator
 *  or by explicitly calling its constructor with a raw pointer.
 *
 *  The raw pointer encapsulated by a \p musa::universal_pointer may be
 *  obtained by eiter its <tt>get</tt> member function or the \p
 *  raw_pointer_cast function.
 *
 *  \note \p musa::universal_pointer is not a "smart" pointer; it is the
 *        programmer's responsibility to deallocate memory pointed to by
 *        \p musa::universal_pointer.
 *
 *  \tparam T specifies the type of the pointee.
 *
 *  \see musa::universal_allocator
 *  \see raw_pointer_cast
 */
template <typename T>
using universal_pointer = thrust::pointer<
  T,
  thrust::cuda_cub::tag,
  typename std::add_lvalue_reference<T>::type
>;

/*! \p musa::reference is a wrapped reference to an object stored in memory
 *  accessible by the \p musa system. \p musa::reference is the type of the
 *  result of dereferencing a \p musa::pointer.
 *
 *  \tparam T Specifies the type of the referenced object.
 *
 *  \see musa::pointer
 */
template <typename T>
using reference = thrust::tagged_reference<T, thrust::cuda_cub::tag>;

} // namespace cuda_cub

/*! \addtogroup system_backends Systems
 *  \ingroup system
 *  \{
 */

/*! \namespace thrust::system::musa
 *  \brief \p thrust::system::musa is the namespace containing functionality
 *  for allocating, manipulating, and deallocating memory available to Thrust's
 *  CUDA backend system. The identifiers are provided in a separate namespace
 *  underneath <tt>thrust::system</tt> for import convenience but are also
 *  aliased in the top-level <tt>thrust::musa</tt> namespace for easy access.
 *
 */
namespace system { namespace musa
{
using thrust::cuda_cub::pointer;
using thrust::cuda_cub::universal_pointer;
using thrust::cuda_cub::reference;
}} // namespace system::musa
/*! \}
 */

/*! \namespace thrust::musa
 *  \brief \p thrust::musa is a top-level alias for \p thrust::system::musa.
 */
namespace musa
{
using thrust::cuda_cub::pointer;
using thrust::cuda_cub::universal_pointer;
using thrust::cuda_cub::reference;
} // namespace musa

} // namespace thrust

