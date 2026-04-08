#include <unittest/unittest.h>
#include <thrust/sort.h>
#include <thrust/functional.h>


template <typename T, unsigned int N>
void _TestStableSortWithLargeKeys(void)
{
    size_t n = (128 * 1024) / sizeof(FixedVector<T,N>);

    thrust::host_vector< FixedVector<T,N> > h_keys(n);

    for(size_t i = 0; i < n; i++)
        // XXX Use proper random number generation facility.
        h_keys[i] = FixedVector<T,N>(rand());

    thrust::device_vector< FixedVector<T,N> > d_keys = h_keys;
    
    thrust::stable_sort(h_keys.begin(), h_keys.end());
    thrust::stable_sort(d_keys.begin(), d_keys.end());

    ASSERT_EQUAL_QUIET(h_keys, d_keys);
}

void TestStableSortWithLargeKeys(void)
{
    _TestStableSortWithLargeKeys<int,    1>();
    _TestStableSortWithLargeKeys<int,    2>();
    _TestStableSortWithLargeKeys<int,    4>();
    _TestStableSortWithLargeKeys<int,    8>();
    _TestStableSortWithLargeKeys<int,   16>();
#if THRUST_DEVICE_SYSTEM != THRUST_DEVICE_SYSTEM_MUSA || !defined(CUB_MUSA_ARCH) || CUB_MUSA_ARCH >= 310
    _TestStableSortWithLargeKeys<int,   32>();
    _TestStableSortWithLargeKeys<int,   64>();
#endif
#if THRUST_DEVICE_SYSTEM != THRUST_DEVICE_SYSTEM_MUSA
    _TestStableSortWithLargeKeys<int,  128>();
    _TestStableSortWithLargeKeys<int,  256>();
#endif

// XXX these take too long to compile
//    _TestStableSortWithLargeKeys<int,  512>();
//    _TestStableSortWithLargeKeys<int, 1024>();
//    _TestStableSortWithLargeKeys<int, 2048>();
//    _TestStableSortWithLargeKeys<int, 4096>();
//    _TestStableSortWithLargeKeys<int, 8192>();
}
DECLARE_UNITTEST(TestStableSortWithLargeKeys);
