/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#include <unittest/unittest.h>
#include <thrust/execution_policy.h>
#include <thrust/system/musa/detail/util.h>

#include <thread>

void verify_stream()
{
  auto exec = thrust::device;
  auto stream = thrust::cuda_cub::stream(exec);
  ASSERT_EQUAL(stream, musaStreamPerThread);
}

void TestPerThreadDefaultStream()
{
  verify_stream();

  std::thread t(verify_stream);
  t.join();
}
DECLARE_UNITTEST(TestPerThreadDefaultStream);
