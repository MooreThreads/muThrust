/*
 *  Copyright 2018-2020 NVIDIA Corporation
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

/*! \file musa/memory_resource.h
 *  \brief Memory resources for the MUSA system.
 */

#pragma once

#include <thrust/detail/config.h>

#include <thrust/mr/memory_resource.h>
#include <thrust/system/musa/detail/guarded_cuda_runtime_api.h>
#include <thrust/system/musa/pointer.h>
#include <thrust/system/detail/bad_alloc.h>
#include <thrust/system/musa/error.h>
#include <thrust/system/musa/detail/util.h>

#include <thrust/mr/host_memory_resource.h>

THRUST_NAMESPACE_BEGIN

namespace system
{
namespace musa
{

//! \cond
namespace detail
{

    typedef musaError_t (MUSARTAPI *allocation_fn)(void **, std::size_t);
    typedef musaError_t (MUSARTAPI *deallocation_fn)(void *);

    template<allocation_fn Alloc, deallocation_fn Dealloc, typename Pointer>
    class musa_memory_resource final : public mr::memory_resource<Pointer>
    {
    public:
        Pointer do_allocate(std::size_t bytes, std::size_t alignment = THRUST_MR_DEFAULT_ALIGNMENT) override
        {
            (void)alignment;

            void * ret;
            musaError_t status = Alloc(&ret, bytes);

            if (status != musaSuccess)
            {
                musaGetLastError(); // Clear the MUSA global error state.
                throw thrust::system::detail::bad_alloc(thrust::musa_category().message(status).c_str());
            }

            return Pointer(ret);
        }

        void do_deallocate(Pointer p, std::size_t bytes, std::size_t alignment) override
        {
            (void)bytes;
            (void)alignment;

            musaError_t status = Dealloc(thrust::detail::pointer_traits<Pointer>::get(p));

            if (status != musaSuccess)
            {
                thrust::musa_cub::throw_on_error(status, "musa free failed");
            }
        }
    };

    inline musaError_t MUSARTAPI musaMallocManaged(void ** ptr, std::size_t bytes)
    {
        return ::musaMallocManaged(ptr, bytes, musaMemAttachGlobal);
    }

    typedef detail::musa_memory_resource<musaMalloc, musaFree,
        thrust::musa::pointer<void> >
        device_memory_resource;
    typedef detail::musa_memory_resource<detail::musaMallocManaged, musaFree,
        thrust::musa::universal_pointer<void> >
        managed_memory_resource;
    typedef detail::musa_memory_resource<musaMallocHost, musaFreeHost,
        thrust::musa::universal_pointer<void> >
        pinned_memory_resource;

} // end detail
//! \endcond

/*! The memory resource for the MUSA system. Uses <tt>musaMalloc</tt> and wraps
 *  the result with \p musa::pointer.
 */
typedef detail::device_memory_resource memory_resource;
/*! The universal memory resource for the MUSA system. Uses
 *  <tt>musaMallocManaged</tt> and wraps the result with
 *  \p musa::universal_pointer.
 */
typedef detail::managed_memory_resource universal_memory_resource;
/*! The host pinned memory resource for the MUSA system. Uses
 *  <tt>musaMallocHost</tt> and wraps the result with \p
 *  musa::universal_pointer.
 */
typedef detail::pinned_memory_resource universal_host_pinned_memory_resource;

} // end musa
} // end system

namespace musa
{
using thrust::system::musa::memory_resource;
using thrust::system::musa::universal_memory_resource;
using thrust::system::musa::universal_host_pinned_memory_resource;
}

THRUST_NAMESPACE_END

