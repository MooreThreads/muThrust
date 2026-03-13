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


/*! \file thrust/system/musa/error.h
 *  \brief CUDA-specific error reporting
 */

#pragma once

#include <thrust/detail/config.h>
#include <thrust/detail/type_traits.h>
#include <thrust/system/error_code.h>
#include <thrust/system/musa/detail/guarded_driver_types.h>

namespace thrust
{

namespace system
{

namespace musa
{

// To construct an error_code after a CUDA Runtime error:
//
//   error_code(::musaGetLastError(), cuda_category())

// XXX N3000 prefers enum class errc { ... }
/*! Namespace for CUDA Runtime errors.
 */
namespace errc
{

/*! \p errc_t enumerates the kinds of CUDA Runtime errors.
 */
enum errc_t
{
  // from musa/include/driver_types.h
  // mirror their order
  success                            = musaSuccess,
  missing_configuration              = musaErrorMissingConfiguration,
  memory_allocation                  = musaErrorMemoryAllocation,
  initialization_error               = musaErrorInitializationError,
  launch_failure                     = musaErrorLaunchFailure,
  prior_launch_failure               = musaErrorPriorLaunchFailure,
  launch_timeout                     = musaErrorLaunchTimeout,
  launch_out_of_resources            = musaErrorLaunchOutOfResources,
  invalid_device_function            = musaErrorInvalidDeviceFunction,
  invalid_configuration              = musaErrorInvalidConfiguration,
  invalid_device                     = musaErrorInvalidDevice,
  invalid_value                      = musaErrorInvalidValue,
  invalid_pitch_value                = musaErrorInvalidPitchValue,
  invalid_symbol                     = musaErrorInvalidSymbol,
  map_buffer_object_failed           = musaErrorMapBufferObjectFailed,
  unmap_buffer_object_failed         = musaErrorUnmapBufferObjectFailed,
  invalid_host_pointer               = musaErrorInvalidHostPointer,
  invalid_device_pointer             = musaErrorInvalidDevicePointer,
  invalid_texture                    = musaErrorInvalidTexture,
  invalid_texture_binding            = musaErrorInvalidTextureBinding,
  invalid_channel_descriptor         = musaErrorInvalidChannelDescriptor,
  invalid_memcpy_direction           = musaErrorInvalidMemcpyDirection,
  address_of_constant_error          = musaErrorAddressOfConstant,
  texture_fetch_failed               = musaErrorTextureFetchFailed,
  texture_not_bound                  = musaErrorTextureNotBound,
  synchronization_error              = musaErrorSynchronizationError,
  invalid_filter_setting             = musaErrorInvalidFilterSetting,
  invalid_norm_setting               = musaErrorInvalidNormSetting,
  mixed_device_execution             = musaErrorMixedDeviceExecution,
  cuda_runtime_unloading             = musaErrorMusartUnloading,
  unknown                            = musaErrorUnknown,
  not_yet_implemented                = musaErrorNotYetImplemented,
  memory_value_too_large             = musaErrorMemoryValueTooLarge,
  invalid_resource_handle            = musaErrorInvalidResourceHandle,
  not_ready                          = musaErrorNotReady,
  insufficient_driver                = musaErrorInsufficientDriver,
  set_on_active_process_error        = musaErrorSetOnActiveProcess,
  no_device                          = musaErrorNoDevice,
  ecc_uncorrectable                  = musaErrorECCUncorrectable,

#if MUSART_VERSION >= 4020
  shared_object_symbol_not_found     = musaErrorSharedObjectSymbolNotFound,
  shared_object_init_failed          = musaErrorSharedObjectInitFailed,
  unsupported_limit                  = musaErrorUnsupportedLimit,
  duplicate_variable_name            = musaErrorDuplicateVariableName,
  duplicate_texture_name             = musaErrorDuplicateTextureName,
  duplicate_surface_name             = musaErrorDuplicateSurfaceName,
  devices_unavailable                = musaErrorDevicesUnavailable,
  invalid_kernel_image               = musaErrorInvalidKernelImage,
  no_kernel_image_for_device         = musaErrorNoKernelImageForDevice,
  incompatible_driver_context        = musaErrorIncompatibleDriverContext,
  peer_access_already_enabled        = musaErrorPeerAccessAlreadyEnabled,
  peer_access_not_enabled            = musaErrorPeerAccessNotEnabled,
  device_already_in_use              = musaErrorDeviceAlreadyInUse,
  profiler_disabled                  = musaErrorProfilerDisabled,
  assert_triggered                   = musaErrorAssert,
  too_many_peers                     = musaErrorTooManyPeers,
  host_memory_already_registered     = musaErrorHostMemoryAlreadyRegistered,
  host_memory_not_registered         = musaErrorHostMemoryNotRegistered,
  operating_system_error             = musaErrorOperatingSystem,
#endif

#if MUSART_VERSION >= 5000
  peer_access_unsupported            = musaErrorPeerAccessUnsupported,
  launch_max_depth_exceeded          = musaErrorLaunchMaxDepthExceeded,
  launch_file_scoped_texture_used    = musaErrorLaunchFileScopedTex,
  launch_file_scoped_surface_used    = musaErrorLaunchFileScopedSurf,
  sync_depth_exceeded                = musaErrorSyncDepthExceeded,
  attempted_operation_not_permitted  = musaErrorNotPermitted,
  attempted_operation_not_supported  = musaErrorNotSupported,
#endif

  startup_failure                    = musaErrorStartupFailure
}; // end errc_t


} // end namespace errc

} // end namespace cuda_cub

/*! \return A reference to an object of a type derived from class \p thrust::error_category.
 *  \note The object's \p equivalent virtual functions shall behave as specified
 *        for the class \p thrust::error_category. The object's \p name virtual function shall
 *        return a pointer to the string <tt>"musa"</tt>. The object's
 *        \p default_error_condition virtual function shall behave as follows:
 *
 *        If the argument <tt>ev</tt> corresponds to a CUDA error value, the function
 *        shall return <tt>error_condition(ev,cuda_category())</tt>.
 *        Otherwise, the function shall return <tt>system_category.default_error_condition(ev)</tt>.
 */
inline const error_category &cuda_category(void);


// XXX N3000 prefers is_error_code_enum<musa::errc>

/*! Specialization of \p is_error_code_enum for \p musa::errc::errc_t
 */
template<> struct is_error_code_enum<musa::errc::errc_t> : thrust::detail::true_type {};


// XXX replace musa::errc::errc_t with musa::errc upon c++0x
/*! \return <tt>error_code(static_cast<int>(e), musa::error_category())</tt>
 */
inline error_code make_error_code(musa::errc::errc_t e);


// XXX replace musa::errc::errc_t with musa::errc upon c++0x
/*! \return <tt>error_condition(static_cast<int>(e), musa::error_category())</tt>.
 */
inline error_condition make_error_condition(musa::errc::errc_t e);

} // end system

namespace cuda_cub
{
namespace errc = system::musa::errc;
} // end cuda_cub

namespace musa
{
// XXX replace with using system::cuda_errc upon c++0x
namespace errc = system::musa::errc;
} // end musa

using system::cuda_category;

} // end namespace thrust

#include <thrust/system/musa/detail/error.inl>

