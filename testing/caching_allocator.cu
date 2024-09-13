/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#include <unittest/unittest.h>

#include <thrust/detail/config.h>
#include <thrust/detail/caching_allocator.h>

template<typename Allocator>
void test_implementation(Allocator alloc)
{
    typedef typename thrust::detail::allocator_traits<Allocator> Traits;
    typedef typename Allocator::pointer Ptr;

    Ptr p = Traits::allocate(alloc, 123);
    Traits::deallocate(alloc, p, 123);

    Ptr p2 = Traits::allocate(alloc, 123);
    ASSERT_EQUAL(p, p2);
}

void TestSingleDeviceTLSCachingAllocator()
{
    test_implementation(thrust::detail::single_device_tls_caching_allocator());
};
DECLARE_UNITTEST(TestSingleDeviceTLSCachingAllocator);
