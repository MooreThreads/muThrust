/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#include <unittest/unittest.h>
#include <thrust/tabulate.h>
#include <thrust/functional.h>
#include <thrust/execution_policy.h>


template<typename ExecutionPolicy, typename Iterator, typename Function>
__global__
void tabulate_kernel(ExecutionPolicy exec, Iterator first, Iterator last, Function f)
{
  thrust::tabulate(exec, first, last, f);
}


template<typename ExecutionPolicy>
void TestTabulateDevice(ExecutionPolicy exec)
{
  typedef thrust::device_vector<int> Vector;
  using namespace thrust::placeholders;
  typedef typename Vector::value_type T;
  
  Vector v(5);

  tabulate_kernel<<<1,1>>>(exec, v.begin(), v.end(), thrust::identity<T>());
  {
    musaError_t const err = musaDeviceSynchronize();
    ASSERT_EQUAL(musaSuccess, err);
  }

  ASSERT_EQUAL(v[0], 0);
  ASSERT_EQUAL(v[1], 1);
  ASSERT_EQUAL(v[2], 2);
  ASSERT_EQUAL(v[3], 3);
  ASSERT_EQUAL(v[4], 4);

  tabulate_kernel<<<1,1>>>(exec, v.begin(), v.end(), -_1);
  {
    musaError_t const err = musaDeviceSynchronize();
    ASSERT_EQUAL(musaSuccess, err);
  }

  ASSERT_EQUAL(v[0],  0);
  ASSERT_EQUAL(v[1], -1);
  ASSERT_EQUAL(v[2], -2);
  ASSERT_EQUAL(v[3], -3);
  ASSERT_EQUAL(v[4], -4);
  
  tabulate_kernel<<<1,1>>>(exec, v.begin(), v.end(), _1 * _1 * _1);
  {
    musaError_t const err = musaDeviceSynchronize();
    ASSERT_EQUAL(musaSuccess, err);
  }

  ASSERT_EQUAL(v[0], 0);
  ASSERT_EQUAL(v[1], 1);
  ASSERT_EQUAL(v[2], 8);
  ASSERT_EQUAL(v[3], 27);
  ASSERT_EQUAL(v[4], 64);
}

void TestTabulateDeviceSeq()
{
  TestTabulateDevice(thrust::seq);
}
DECLARE_UNITTEST(TestTabulateDeviceSeq);

void TestTabulateDeviceDevice()
{
  TestTabulateDevice(thrust::device);
}
DECLARE_UNITTEST(TestTabulateDeviceDevice);

void TestTabulateCudaStreams()
{
  using namespace thrust::placeholders;
  typedef thrust::device_vector<int> Vector;
  typedef Vector::value_type T;
  
  Vector v(5);

  musaStream_t s;
  musaStreamCreate(&s);

  thrust::tabulate(thrust::musa::par.on(s), v.begin(), v.end(), thrust::identity<T>());
  musaStreamSynchronize(s);

  ASSERT_EQUAL(v[0], 0);
  ASSERT_EQUAL(v[1], 1);
  ASSERT_EQUAL(v[2], 2);
  ASSERT_EQUAL(v[3], 3);
  ASSERT_EQUAL(v[4], 4);

  thrust::tabulate(thrust::musa::par.on(s), v.begin(), v.end(), -_1);
  musaStreamSynchronize(s);

  ASSERT_EQUAL(v[0],  0);
  ASSERT_EQUAL(v[1], -1);
  ASSERT_EQUAL(v[2], -2);
  ASSERT_EQUAL(v[3], -3);
  ASSERT_EQUAL(v[4], -4);
  
  thrust::tabulate(thrust::musa::par.on(s), v.begin(), v.end(), _1 * _1 * _1);
  musaStreamSynchronize(s);

  ASSERT_EQUAL(v[0], 0);
  ASSERT_EQUAL(v[1], 1);
  ASSERT_EQUAL(v[2], 8);
  ASSERT_EQUAL(v[3], 27);
  ASSERT_EQUAL(v[4], 64);

  musaStreamSynchronize(s);
}
DECLARE_UNITTEST(TestTabulateCudaStreams);

