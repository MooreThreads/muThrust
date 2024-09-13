/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#include <thrust/detail/config.h>
#include <thrust/device_vector.h>

// If you create a global `thrust::device_vector` with the default allocator,
// you'll get an error during program termination when the memory of the vector
// is freed, as the CUDA runtime cannot be used during program termination.
//
// To get around this, you can create your own allocator which ignores
// deallocation failures that occur because the CUDA runtime is shut down.

extern "C" musaError_t cudaFreeIgnoreShutdown(void* ptr) {
  musaError_t const err = musaFree(ptr);
  if (musaSuccess == err || musaErrorMusartUnloading == err)
    return musaSuccess;
  return err; 
}

typedef thrust::system::musa::detail::cuda_memory_resource<
  musaMalloc, 
  cudaFreeIgnoreShutdown,
  thrust::musa::pointer<void>
> device_ignore_shutdown_memory_resource;

#if THRUST_CPP_DIALECT >= 2011
  template <typename T>
  using device_ignore_shutdown_allocator = 
    thrust::mr::stateless_resource_allocator<
      T,
      thrust::device_ptr_memory_resource<device_ignore_shutdown_memory_resource>
    >;
    
  thrust::device_vector<double, device_ignore_shutdown_allocator<double>> d;
#else
  thrust::device_vector<
    double, 
    thrust::mr::stateless_resource_allocator<
      double,
      thrust::device_ptr_memory_resource<device_ignore_shutdown_memory_resource>
    > 
  > d;
#endif

int main() {
  d.resize(25);
}

