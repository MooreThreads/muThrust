/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#include <unittest/unittest.h>
#include <thrust/device_vector.h>
#include <thrust/device_ptr.h>
#include <thrust/device_new.h>
#include <thrust/device_delete.h>

struct Foo
{
  __host__ __device__
  Foo(void)
    :set_me_upon_destruction(0)
  {}

  __host__ __device__
  ~Foo(void)
  {
#if defined(__CUDA_ARCH__) || defined(__MUSA_ARCH__)
    // __device__ overload
    if(set_me_upon_destruction != 0)
      *set_me_upon_destruction = 1;
#endif
  }

  int *set_me_upon_destruction;
};

#if !defined(__QNX__)
void TestDeviceDeleteDestructorInvocation(void)
{
  thrust::device_vector<int> destructor_flag(1, 0);

  thrust::device_ptr<Foo> foo_ptr = thrust::device_new<Foo>();

  Foo exemplar;
  exemplar.set_me_upon_destruction =
    thrust::raw_pointer_cast(destructor_flag.data());
  *foo_ptr = exemplar;

  // MUSA assignment to device_reference may destroy internal temporaries on the
  // device. Clear the observation flag so this test only checks device_delete.
  destructor_flag[0] = 0;

  ASSERT_EQUAL(0, destructor_flag[0]);

  thrust::device_delete(foo_ptr);

  ASSERT_EQUAL(1, destructor_flag[0]);
}
DECLARE_UNITTEST(TestDeviceDeleteDestructorInvocation);
#endif
