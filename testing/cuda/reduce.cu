/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#include <unittest/unittest.h>
#include <thrust/reduce.h>
#include <thrust/execution_policy.h>


template<typename ExecutionPolicy, typename Iterator, typename T, typename Iterator2>
__global__
void reduce_kernel(ExecutionPolicy exec, Iterator first, Iterator last, T init, Iterator2 result)
{
  *result = thrust::reduce(exec, first, last, init);
}


template<typename T, typename ExecutionPolicy>
void TestReduceDevice(ExecutionPolicy exec, const size_t n)
{
  thrust::host_vector<T>   h_data = unittest::random_integers<T>(n);
  thrust::device_vector<T> d_data = h_data;
  
  thrust::device_vector<T> d_result(1);
  
  T init = 13;
  
  T h_result = thrust::reduce(h_data.begin(), h_data.end(), init);
  
  reduce_kernel<<<1,1>>>(exec, d_data.begin(), d_data.end(), init, d_result.begin());
  musaError_t const err = musaDeviceSynchronize();
  ASSERT_EQUAL(musaSuccess, err);
  
  ASSERT_EQUAL(h_result, d_result[0]);
}


template<typename T>
struct TestReduceDeviceSeq
{
  void operator()(const size_t n)
  {
    TestReduceDevice<T>(thrust::seq, n);
  }
};
VariableUnitTest<TestReduceDeviceSeq, IntegralTypes> TestReduceDeviceSeqInstance;


template<typename T>
struct TestReduceDeviceDevice
{
  void operator()(const size_t n)
  {
    TestReduceDevice<T>(thrust::device, n);
  }
};
VariableUnitTest<TestReduceDeviceDevice, IntegralTypes> TestReduceDeviceDeviceInstance;


void TestReduceCudaStreams()
{
  typedef thrust::device_vector<int> Vector;

  Vector v(3);
  v[0] = 1; v[1] = -2; v[2] = 3;

  musaStream_t s;
  musaStreamCreate(&s);

  // no initializer
  ASSERT_EQUAL(thrust::reduce(thrust::musa::par.on(s), v.begin(), v.end()), 2);

  // with initializer
  ASSERT_EQUAL(thrust::reduce(thrust::musa::par.on(s), v.begin(), v.end(), 10), 12);

  musaStreamDestroy(s);
}
DECLARE_UNITTEST(TestReduceCudaStreams);

