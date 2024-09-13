/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#pragma once

#include <unittest/testframework.h>
#include <thrust/system/musa/memory.h>
#include <thrust/system_error.h>
#include <vector>

class CUDATestDriver
  : public UnitTestDriver
{
  public:
    int current_device_architecture() const;

  private:
    std::vector<int> target_devices(const ArgumentMap &kwargs);

    bool check_cuda_error(bool concise);

    virtual bool post_test_smoke_check(const UnitTest &test, bool concise);

    virtual bool run_tests(const ArgumentSet &args, const ArgumentMap &kwargs);
};

UnitTestDriver &driver_instance(thrust::system::musa::tag);

