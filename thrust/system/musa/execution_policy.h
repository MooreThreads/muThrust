/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
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

// histogram
// sort (radix-sort, merge-sort)

#include <thrust/detail/config.h>
#include <thrust/system/musa/detail/execution_policy.h>
#include <thrust/system/musa/detail/par.h>

// pass
// ----------------
#include <thrust/system/musa/detail/adjacent_difference.h>
#include <thrust/system/musa/detail/copy.h>
#include <thrust/system/musa/detail/copy_if.h>
#include <thrust/system/musa/detail/count.h>
#include <thrust/system/musa/detail/equal.h>
#include <thrust/system/musa/detail/extrema.h>
#include <thrust/system/musa/detail/fill.h>
#include <thrust/system/musa/detail/find.h>
#include <thrust/system/musa/detail/for_each.h>
#include <thrust/system/musa/detail/gather.h>
#include <thrust/system/musa/detail/generate.h>
#include <thrust/system/musa/detail/inner_product.h>
#include <thrust/system/musa/detail/mismatch.h>
#include <thrust/system/musa/detail/partition.h>
#include <thrust/system/musa/detail/reduce_by_key.h>
#include <thrust/system/musa/detail/remove.h>
#include <thrust/system/musa/detail/replace.h>
#include <thrust/system/musa/detail/reverse.h>
#include <thrust/system/musa/detail/scatter.h>
#include <thrust/system/musa/detail/swap_ranges.h>
#include <thrust/system/musa/detail/tabulate.h>
#include <thrust/system/musa/detail/transform.h>
#include <thrust/system/musa/detail/transform_reduce.h>
#include <thrust/system/musa/detail/transform_scan.h>
#include <thrust/system/musa/detail/uninitialized_copy.h>
#include <thrust/system/musa/detail/uninitialized_fill.h>
#include <thrust/system/musa/detail/unique.h>
#include <thrust/system/musa/detail/unique_by_key.h>

// fail
// ----------------
// fails with mixed types
#include <thrust/system/musa/detail/reduce.h>

// mixed types are not compiling, commented in testing/scan.mu
#include <thrust/system/musa/detail/scan.h>

// stubs passed
// ----------------
#include <thrust/system/musa/detail/binary_search.h>
#include <thrust/system/musa/detail/merge.h>
#include <thrust/system/musa/detail/scan_by_key.h>
#include <thrust/system/musa/detail/set_operations.h>
#include <thrust/system/musa/detail/sort.h>

// work in progress

