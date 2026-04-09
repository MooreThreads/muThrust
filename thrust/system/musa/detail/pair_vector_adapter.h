#pragma once

#include <thrust/detail/config.h>
#include <thrust/pair.h>
#include <thrust/system/musa/detail/guarded_cuda_runtime_api.h>

#include <type_traits>

THRUST_NAMESPACE_BEGIN
namespace musa_cub
{
namespace detail
{

template <typename PairT>
struct pair_vector_adapter
{
  static constexpr bool value = false;
};

template <typename T>
struct pair_storage_type;

template <>
struct pair_storage_type<signed char>
{
  using type = char2;
};

template <>
struct pair_storage_type<unsigned char>
{
  using type = uchar2;
};

template <>
struct pair_storage_type<short>
{
  using type = short2;
};

template <>
struct pair_storage_type<unsigned short>
{
  using type = ushort2;
};

template <>
struct pair_storage_type<int>
{
  using type = int2;
};

template <>
struct pair_storage_type<unsigned int>
{
  using type = uint2;
};

template <>
struct pair_storage_type<long long>
{
  using type = longlong2;
};

template <>
struct pair_storage_type<unsigned long long>
{
  using type = ulonglong2;
};

template <typename T>
struct pair_vector_adapter<thrust::pair<T, T>>
{
  static constexpr bool value =
    std::is_arithmetic<T>::value &&
    !std::is_same<T, bool>::value &&
    (sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);

  using pair_type = thrust::pair<T, T>;
  using storage_type = typename pair_storage_type<T>::type;

  __host__ __device__ static pair_type to_pair(storage_type value)
  {
    return thrust::make_pair(value.x, value.y);
  }

  __host__ __device__ static storage_type to_storage(pair_type value)
  {
    storage_type storage{};
    storage.x = value.first;
    storage.y = value.second;
    return storage;
  }
};

template <typename T>
struct is_small_pair_vectorizable
    : std::integral_constant<
        bool,
        pair_vector_adapter<T>::value && (sizeof(T) <= 4)>
{
};

template <typename ScanOp, typename PairT>
struct pair_vector_scan_op
{
  using adapter = pair_vector_adapter<PairT>;
  using storage_type = typename adapter::storage_type;

  mutable ScanOp scan_op;

  __host__ __device__ storage_type operator()(storage_type lhs,
                                              storage_type rhs) const
  {
    const PairT pair_result = scan_op(adapter::to_pair(lhs), adapter::to_pair(rhs));
    return adapter::to_storage(pair_result);
  }
};

template <typename CompareOp, typename PairT>
struct pair_vector_compare_op
{
  using adapter = pair_vector_adapter<PairT>;
  using storage_type = typename adapter::storage_type;

  mutable CompareOp compare_op;

  __host__ __device__ bool operator()(storage_type lhs, storage_type rhs) const
  {
    return compare_op(adapter::to_pair(lhs), adapter::to_pair(rhs));
  }
};

template <typename PairT>
__host__ __device__ typename pair_vector_adapter<PairT>::storage_type*
pair_storage_pointer(PairT* ptr)
{
  using storage_type = typename pair_vector_adapter<PairT>::storage_type;
  return reinterpret_cast<storage_type*>(ptr);
}

template <typename PairT>
__host__ __device__ const typename pair_vector_adapter<PairT>::storage_type*
pair_storage_pointer(const PairT* ptr)
{
  using storage_type = typename pair_vector_adapter<PairT>::storage_type;
  return reinterpret_cast<const storage_type*>(ptr);
}

} // namespace detail
} // namespace musa_cub
THRUST_NAMESPACE_END
