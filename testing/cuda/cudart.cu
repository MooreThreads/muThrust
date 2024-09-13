/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#include <unittest/unittest.h>
#include <musa_runtime_api.h>
#include <thrust/detail/util/align.h>

template<typename T>
void TestCudaMallocResultAligned(const std::size_t n)
{
  T *ptr = 0;
  musaMalloc(&ptr, n * sizeof(T));
  musaFree(ptr);

  ASSERT_EQUAL(true, thrust::detail::util::is_aligned(ptr));
}
DECLARE_VARIABLE_UNITTEST(TestCudaMallocResultAligned);

