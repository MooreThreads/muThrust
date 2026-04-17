/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#include <thrust/device_vector.h>
#include <thrust/complex.h>
#include <thrust/reduce.h>

int main()
{
  thrust::device_vector<thrust::complex<double> > d(5);
  thrust::reduce(d.begin(), d.end());
}

