/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
/*
 *  Copyright 2008-2009 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

// A simple timer class

#ifdef __MUSACC__

// use CUDA's high-resolution timers when possible
#include <musa_runtime_api.h>
#include <thrust/system/musa/error.h>
#include <thrust/system_error.h>
#include <string>

void cuda_safe_call(musaError_t error, const std::string& message = "")
{
  if(error)
    throw thrust::system_error(error, thrust::cuda_category(), message);
}

struct timer
{
  musaEvent_t start;
  musaEvent_t end;

  timer(void)
  {
    cuda_safe_call(musaEventCreate(&start));
    cuda_safe_call(musaEventCreate(&end));
    restart();
  }

  ~timer(void)
  {
    cuda_safe_call(musaEventDestroy(start));
    cuda_safe_call(musaEventDestroy(end));
  }

  void restart(void)
  {
    cuda_safe_call(musaEventRecord(start, 0));
  }

  double elapsed(void)
  {
    cuda_safe_call(musaEventRecord(end, 0));
    cuda_safe_call(musaEventSynchronize(end));

    float ms_elapsed;
    cuda_safe_call(musaEventElapsedTime(&ms_elapsed, start, end));
    return ms_elapsed / 1e3;
  }

  double epsilon(void)
  {
    return 0.5e-6;
  }
};

#else

// fallback to clock()
#include <ctime>

struct timer
{
  clock_t start;
  clock_t end;

  timer(void)
  {
    restart();
  }

  ~timer(void)
  {
  }

  void restart(void)
  {
    start = clock();
  }

  double elapsed(void)
  {
    end = clock();

    return static_cast<double>(end - start) / static_cast<double>(CLOCKS_PER_SEC);
  }

  double epsilon(void)
  {
    return 1.0 / static_cast<double>(CLOCKS_PER_SEC);
  }
};

#endif

