/******************************************************************************
 * Copyright (c) 2016, NVIDIA CORPORATION.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the NVIDIA CORPORATION nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/
#pragma once

#include <thrust/detail/config.h>
#include <thrust/system/musa/config.h>
#include <thrust/system/musa/tag.h>
#include <thrust/detail/execution_policy.h>

THRUST_NAMESPACE_BEGIN

namespace musa
{

// 前向声明
struct execution_policy;

// MUSA 执行策略基类
template<typename Derived>
struct execution_policy : thrust::detail::execution_policy_base<Derived>
{
  typedef thrust::musa::tag tag_type;
  
  __host__ __device__
  execution_policy() : thrust::detail::execution_policy_base<Derived>() {}
};

// 具体执行策略
struct policy : execution_policy<policy>
{
  __host__ __device__
  policy() : execution_policy<policy>() {}
};

// par 执行策略（并行）
struct par_t : execution_policy<par_t>
{
  __host__ __device__
  par_t() : execution_policy<par_t>() {}
};

// seq 执行策略（串行）
struct seq_t : execution_policy<seq_t>
{
  __host__ __device__
  seq_t() : execution_policy<seq_t>() {}
};

// 全局执行策略实例
__host__ __device__
inline const par_t par() { return par_t(); }

__host__ __device__
inline const seq_t seq() { return seq_t(); }

} // end namespace musa

// 定义默认执行策略
using execution_policy_t = policy;

THRUST_NAMESPACE_END