/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#include <unittest/unittest.h>
#include <thrust/system/musa/experimental/pinned_allocator.h>
#include <thrust/host_vector.h>
#include <thrust/copy.h>

template <typename T>
void TestPinnedAllocatorSimple(const size_t n)
{
  typedef thrust::host_vector<T, thrust::musa::experimental::pinned_allocator<T> > Vector;

  Vector h_input = unittest::random_integers<T>(n);
  Vector h_output(n);

  thrust::copy(h_input.begin(), h_input.end(), h_output.begin());

  ASSERT_EQUAL(h_input, h_output);
}
DECLARE_VARIABLE_UNITTEST(TestPinnedAllocatorSimple);

