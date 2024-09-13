/****************************************************************************
* This library contains code from thrust, thrust is licensed under the license
* below.
* Some files of thrust may have been modified by Moore Threads Technology Co.
* , Ltd
******************************************************************************/
#pragma once

// for demangling the result of type_info.name()
// with msvc, type_info.name() is already demangled
#ifdef __GNUC__
#include <cxxabi.h>
#endif // __GNUC__

#include <string>
#include <cstdlib>

namespace unittest
{

#if __GNUC__ && !__NVCOMPILER_CUDA__
inline std::string demangle(const char* name)
{
  int status = 0;
  char* realname = abi::__cxa_demangle(name, 0, 0, &status);
  std::string result(realname);
  std::free(realname);

  return result;
}
#else
inline std::string demangle(const char* name)
{
  return name;
}
#endif

} // end unittest

