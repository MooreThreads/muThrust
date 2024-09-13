/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#include <unittest/unittest.h>
#include <thrust/sequence.h>
#include <thrust/execution_policy.h>


template<typename ExecutionPolicy, typename Iterator>
__global__
void sequence_kernel(ExecutionPolicy exec, Iterator first, Iterator last)
{
  thrust::sequence(exec, first, last);
}


template<typename ExecutionPolicy, typename Iterator, typename T>
__global__
void sequence_kernel(ExecutionPolicy exec, Iterator first, Iterator last, T init)
{
  thrust::sequence(exec, first, last, init);
}


template<typename ExecutionPolicy, typename Iterator, typename T>
__global__
void sequence_kernel(ExecutionPolicy exec, Iterator first, Iterator last, T init, T step)
{
  thrust::sequence(exec, first, last, init, step);
}


template<typename ExecutionPolicy>
void TestSequenceDevice(ExecutionPolicy exec)
{
  thrust::device_vector<int> v(5);
  
  sequence_kernel<<<1,1>>>(exec, v.begin(), v.end());
  {
    musaError_t const err = musaDeviceSynchronize();
    ASSERT_EQUAL(musaSuccess, err);
  }
 
  ASSERT_EQUAL(v[0], 0);
  ASSERT_EQUAL(v[1], 1);
  ASSERT_EQUAL(v[2], 2);
  ASSERT_EQUAL(v[3], 3);
  ASSERT_EQUAL(v[4], 4);
  
  sequence_kernel<<<1,1>>>(exec, v.begin(), v.end(), 10);
  {
    musaError_t const err = musaDeviceSynchronize();
    ASSERT_EQUAL(musaSuccess, err);
  }
  
  ASSERT_EQUAL(v[0], 10);
  ASSERT_EQUAL(v[1], 11);
  ASSERT_EQUAL(v[2], 12);
  ASSERT_EQUAL(v[3], 13);
  ASSERT_EQUAL(v[4], 14);
  
  sequence_kernel<<<1,1>>>(exec, v.begin(), v.end(), 10, 2);
  {
    musaError_t const err = musaDeviceSynchronize();
    ASSERT_EQUAL(musaSuccess, err);
  }
  
  ASSERT_EQUAL(v[0], 10);
  ASSERT_EQUAL(v[1], 12);
  ASSERT_EQUAL(v[2], 14);
  ASSERT_EQUAL(v[3], 16);
  ASSERT_EQUAL(v[4], 18);
}

void TestSequenceDeviceSeq()
{
  TestSequenceDevice(thrust::seq);
}
DECLARE_UNITTEST(TestSequenceDeviceSeq);

void TestSequenceDeviceDevice()
{
  TestSequenceDevice(thrust::device);
}
DECLARE_UNITTEST(TestSequenceDeviceDevice);

void TestSequenceCudaStreams()
{
  typedef thrust::device_vector<int> Vector;
  
  Vector v(5);

  musaStream_t s;
  musaStreamCreate(&s);

  thrust::sequence(thrust::musa::par.on(s), v.begin(), v.end());
  musaStreamSynchronize(s);

  ASSERT_EQUAL(v[0], 0);
  ASSERT_EQUAL(v[1], 1);
  ASSERT_EQUAL(v[2], 2);
  ASSERT_EQUAL(v[3], 3);
  ASSERT_EQUAL(v[4], 4);

  thrust::sequence(thrust::musa::par.on(s), v.begin(), v.end(), 10);
  musaStreamSynchronize(s);

  ASSERT_EQUAL(v[0], 10);
  ASSERT_EQUAL(v[1], 11);
  ASSERT_EQUAL(v[2], 12);
  ASSERT_EQUAL(v[3], 13);
  ASSERT_EQUAL(v[4], 14);
  
  thrust::sequence(thrust::musa::par.on(s), v.begin(), v.end(), 10, 2);
  musaStreamSynchronize(s);

  ASSERT_EQUAL(v[0], 10);
  ASSERT_EQUAL(v[1], 12);
  ASSERT_EQUAL(v[2], 14);
  ASSERT_EQUAL(v[3], 16);
  ASSERT_EQUAL(v[4], 18);

  musaStreamDestroy(s);
}
DECLARE_UNITTEST(TestSequenceCudaStreams);

