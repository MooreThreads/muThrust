/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
// Copyright (c) 2018 NVIDIA Corporation
// Author: Bryce Adelstein Lelbach <brycelelbach@gmail.com>
//
// Distributed under the Boost Software License v1.0 (boost.org/LICENSE_1_0.txt)

#pragma once

#include <thrust/detail/config.h>
#include <thrust/detail/cpp14_required.h>

#if THRUST_CPP_DIALECT >= 2014

#include <thrust/system/musa/pointer.h>
#include <thrust/system/musa/detail/execution_policy.h>

namespace thrust
{

namespace system { namespace musa
{

struct ready_event;

template <typename T>
struct ready_future;

struct unique_eager_event;

template <typename T>
struct unique_eager_future;

template <typename... Events>
__host__
unique_eager_event when_all(Events&&... evs);

}} // namespace system::musa

namespace musa
{

using thrust::system::musa::ready_event;

using thrust::system::musa::ready_future;

using thrust::system::musa::unique_eager_event;
using event = unique_eager_event;

using thrust::system::musa::unique_eager_future;
template <typename T> using future = unique_eager_future<T>;

using thrust::system::musa::when_all;

} // namespace musa

template <typename DerivedPolicy>
__host__ 
thrust::musa::unique_eager_event
unique_eager_event_type(
  thrust::musa::execution_policy<DerivedPolicy> const&
) noexcept;

template <typename T, typename DerivedPolicy>
__host__ 
thrust::musa::unique_eager_future<T>
unique_eager_future_type(
  thrust::musa::execution_policy<DerivedPolicy> const&
) noexcept;

} // end namespace thrust

#include <thrust/system/musa/detail/future.inl>

#endif // C++14

