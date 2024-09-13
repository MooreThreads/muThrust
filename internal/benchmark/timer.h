/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#pragma once

#include <cassert>

#  define CUDA_SAFE_CALL_NO_SYNC( call) do {                                 \
    musaError err = call;                                                    \
    if( musaSuccess != err) {                                                \
        fprintf(stderr, "CUDA error in file '%s' in line %i : %s.\n",        \
                __FILE__, __LINE__, musaGetErrorString( err) );              \
        exit(EXIT_FAILURE);                                                  \
    } } while (0)

#  define CUDA_SAFE_CALL( call) do {                                         \
    CUDA_SAFE_CALL_NO_SYNC(call);                                            \
    musaError err = musaDeviceSynchronize();                                 \
    if( musaSuccess != err) {                                                \
        fprintf(stderr, "CUDA error in file '%s' in line %i : %s.\n",        \
                __FILE__, __LINE__, musaGetErrorString( err) );              \
        exit(EXIT_FAILURE);                                                  \
    } } while (0)

class cuda_timer
{
    musaEvent_t start_;
    musaEvent_t stop_;

 public:
    cuda_timer()
    {
        CUDA_SAFE_CALL(musaEventCreate(&start_));
        CUDA_SAFE_CALL(musaEventCreate(&stop_));
    }

    ~cuda_timer()
    {
        CUDA_SAFE_CALL(musaEventDestroy(start_));
        CUDA_SAFE_CALL(musaEventDestroy(stop_));
    }

    void start()
    {
        CUDA_SAFE_CALL(musaEventRecord(start_, 0));
    }

    void stop()
    {
        CUDA_SAFE_CALL(musaEventRecord(stop_, 0));
        CUDA_SAFE_CALL(musaEventSynchronize(stop_));
    }

    double milliseconds_elapsed()
    {
        float elapsed_time;
        CUDA_SAFE_CALL(musaEventElapsedTime(&elapsed_time, start_, stop_));
        return elapsed_time;
    }

    double seconds_elapsed()
    {
        return milliseconds_elapsed() / 1000.0;
    }
};

#if (THRUST_HOST_COMPILER == THRUST_HOST_COMPILER_MSVC)
#include <windows.h>

class steady_timer
{
    LARGE_INTEGER frequency_; // Cached to avoid system calls.
    LARGE_INTEGER start_;
    LARGE_INTEGER stop_;

 public:
    steady_timer() : start_(), stop_(), frequency_()
    {
        BOOL const r = QueryPerformanceFrequency(&frequency_);
        assert(0 != r);
    }

    void start()
    {
        BOOL const r = QueryPerformanceCounter(&start_);
        assert(0 != r);
    }

    void stop()
    {
        BOOL const r = QueryPerformanceCounter(&stop_);
        assert(0 != r);
    }

    double seconds_elapsed()
    {
        return double(stop_.QuadPart - start_.QuadPart)
             / double(frequency_.QuadPart);
    }
};
#else
#include <time.h>

class steady_timer
{
    timespec start_;
    timespec stop_;

 public:
    steady_timer() : start_(), stop_() {}

    void start()
    {
        int const r = clock_gettime(CLOCK_MONOTONIC, &start_);
        assert(0 == r);
    }

    void stop()
    {
        int const r = clock_gettime(CLOCK_MONOTONIC, &stop_);
        assert(0 == r);
    }

    double seconds_elapsed()
    {
        return double(stop_.tv_sec  - start_.tv_sec)
             + double(stop_.tv_nsec - start_.tv_nsec) * 1.0e-9;
    }
};
#endif


