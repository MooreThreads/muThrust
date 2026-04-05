#include <unittest/unittest.h>
#include <thrust/execution_policy.h>
#include <thrust/system/musa/detail/util.h>

#include <thread>

void verify_stream()
{
  auto exec = thrust::device;
  auto stream = thrust::musa_cub::stream(exec);
  ASSERT_EQUAL(stream, musaStreamLegacy);
}

void TestLegacyDefaultStream()
{
  verify_stream();

  std::thread t(verify_stream);
  t.join();
}
DECLARE_UNITTEST(TestLegacyDefaultStream);
