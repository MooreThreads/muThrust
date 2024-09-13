/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#include <thrust/device_ptr.h>
#include <thrust/fill.h>
#include <musa.h>

int main(void)
{
    size_t N = 10;

    // obtain raw pointer to device memory
    int * raw_ptr;
    musaMalloc((void **) &raw_ptr, N * sizeof(int));

    // wrap raw pointer with a device_ptr 
    thrust::device_ptr<int> dev_ptr = thrust::device_pointer_cast(raw_ptr);

    // use device_ptr in Thrust algorithms
    thrust::fill(dev_ptr, dev_ptr + N, (int) 0);

    // access device memory transparently through device_ptr
    dev_ptr[0] = 1;

    // free memory
    musaFree(raw_ptr);

    return 0;
}
