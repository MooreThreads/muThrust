
# 检测是否使用 MUSA 编译器
if (DEFINED CMAKE_MUSA_COMPILER OR MUSA_FOUND)
  set(THRUST_KNOWN_COMPUTE_ARCHS 21 22 31)
  message(STATUS "Thrust: Using MUSA compute architectures: mp_21 mp_22 mp_31")
else()
  set(THRUST_KNOWN_COMPUTE_ARCHS 35 37 50 52 53 60 61 62 70 72 75 80 86)
endif()

# Split CUDA_FLAGS into 3 parts:
#
# THRUST_MUSA_FLAGS_BASE: Common CUDA flags for all targets.
# THRUST_MUSA_FLAGS_RDC: Additional CUDA flags for targets compiled with RDC.
# THRUST_MUSA_FLAGS_NO_RDC: Additional CUDA flags for targets compiled without RDC.
#
# This is necessary because CUDA SMs 5.3, 6.2, and 7.2 do not support RDC, but
# we want to always build some targets (e.g. testing/musa/*) with RDC.
# We work around this by building the "always RDC" targets without support for
# those SMs. This requires two sets of CUDA_FLAGS.
#
# Enabling any of those SMs along with the ENABLE_RDC options will result in a
# configuration error.
#
# Because of how CMake handles the CMAKE_CUDA_FLAGS variables, every target
# generated in a given directory will use the same value for CMAKE_CUDA_FLAGS,
# which is determined at the end of the directory's scope. This means caution
# should be used when trying to build different targets with different flags,
# since they might not behave as expected. This will improve with CMake 3.18,
# which add the DEVICE_LINK genex, fixing the issue with using per-target
# CUDA_FLAGS: https://gitlab.kitware.com/cmake/cmake/-/issues/18265
set(THRUST_MUSA_FLAGS_BASE "${CMAKE_CUDA_FLAGS}")
set(THRUST_MUSA_FLAGS_RDC)
set(THRUST_MUSA_FLAGS_NO_RDC)

# Archs that don't support RDC:
set(no_rdc_archs 53 62 72)

# Find the highest arch:
list(SORT THRUST_KNOWN_COMPUTE_ARCHS)
list(LENGTH THRUST_KNOWN_COMPUTE_ARCHS max_idx)
math(EXPR max_idx "${max_idx} - 1")
list(GET THRUST_KNOWN_COMPUTE_ARCHS ${max_idx} highest_arch)

option(THRUST_AUTO_DETECT_COMPUTE_ARCHS
  "If ON, compute architectures for all GPUs in the current system are enabled and all other compute architectures are disabled."
  OFF
)

if (THRUST_AUTO_DETECT_COMPUTE_ARCHS)
  if ("NVCXX" STREQUAL "${CMAKE_CUDA_COMPILER_ID}")
    message(STATUS "Thrust: Using NVC++ builtin automatic compute architecture detection.")
  else()
    set(detect_compute_archs_source ${Thrust_SOURCE_DIR}/cmake/detect_compute_archs.cu)
    set(detect_compute_archs_exe ${PROJECT_BINARY_DIR}/detect_compute_archs)
    set(detect_compute_archs_error_log ${PROJECT_BINARY_DIR}/detect_compute_archs.stderr.log)
    execute_process(
      COMMAND ${CMAKE_CUDA_COMPILER}
        -std=c++11
        -o ${detect_compute_archs_exe}
        --run
        ${detect_compute_archs_source}
      OUTPUT_VARIABLE detected_archs
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_FILE ${detect_compute_archs_error_log})
    if ("NONE" STREQUAL "${detected_archs}")
      set(detected_message " none")
    else()
      foreach (arch IN LISTS detected_archs)
        string(APPEND detected_message " sm_${arch}")
      endforeach()
    endif()
    message(STATUS "Thrust: Automatically detected compute architectures:${detected_message}")
  endif()
endif()

set(option_init OFF)
if ("NVCXX" STREQUAL "${CMAKE_CUDA_COMPILER_ID}")
  set(option_init ON)
endif()
# MUSA: Default to disabling all architectures, user must explicitly enable
if (DEFINED CMAKE_MUSA_COMPILER OR MUSA_FOUND)
  set(option_init ON)
endif()
option(THRUST_DISABLE_ARCH_BY_DEFAULT
  "If ON, then all compute architectures are disabled on the initial CMake run."
  ${option_init}
)

set(option_init ON)
if (THRUST_DISABLE_ARCH_BY_DEFAULT OR THRUST_AUTO_DETECT_COMPUTE_ARCHS)
  set(option_init OFF)
endif()

set(num_archs_enabled 0)
foreach (arch IN LISTS THRUST_KNOWN_COMPUTE_ARCHS)
  set(this_option_init ${option_init})

  if (${arch} IN_LIST detected_archs)
    set(this_option_init ON)
  endif()

  option(THRUST_ENABLE_COMPUTE_${arch}
    "Enable code generation for tests for sm_${arch}"
    ${this_option_init}
  )

  if (NOT THRUST_ENABLE_COMPUTE_${arch})
    continue()
  endif()

  math(EXPR num_archs_enabled "${num_archs_enabled} + 1")

  if ("NVCXX" STREQUAL "${CMAKE_CUDA_COMPILER_ID}")
    if (NOT ${num_archs_enabled} EQUAL 1)
      message(FATAL_ERROR
        "NVCXX does not support compilation for multiple device architectures "
        "at once."
      )
    endif()
    set(arch_flag "-gpu=cc${arch}")
  elseif (DEFINED CMAKE_MUSA_COMPILER OR MUSA_FOUND)
    # MUSA 使用不同的架构标志格式
    set(arch_flag "--offload-arch=mp_${arch}")
  else()
    set(arch_flag "-gencode arch=compute_${arch},code=sm_${arch}")
  endif()

  if (DEFINED CMAKE_MUSA_COMPILER OR MUSA_FOUND)
    string(APPEND compute_message " mp_${arch}")
  else()
    string(APPEND compute_message " sm_${arch}")
  endif()
  string(APPEND THRUST_MUSA_FLAGS_NO_RDC " ${arch_flag}")
  if (NOT arch IN_LIST no_rdc_archs)
    string(APPEND THRUST_MUSA_FLAGS_RDC " ${arch_flag}")
  endif()
endforeach()

if (NOT "NVCXX" STREQUAL "${CMAKE_CUDA_COMPILER_ID}")
  option(THRUST_ENABLE_COMPUTE_FUTURE
    "Enable code generation for tests for compute_${highest_arch}"
    ${option_init}
  )
  if (THRUST_ENABLE_COMPUTE_FUTURE)
    if (DEFINED CMAKE_MUSA_COMPILER OR MUSA_FOUND)
      # MUSA 不需要 compute_future
    else()
      string(APPEND THRUST_MUSA_FLAGS_BASE
        " -gencode arch=compute_${highest_arch},code=compute_${highest_arch}"
      )
      string(APPEND compute_message " compute_${highest_arch}")
    endif()
  endif()
endif()

message(STATUS "Thrust: Explicitly enabled compute architectures:${compute_message}")

# RDC is off by default in NVCC and on by default in NVC++. Turning off RDC
# isn't currently supported by NVC++. So, we default to RDC off for NVCC and
# RDC on for NVC++.
set(option_init OFF)
if ("NVCXX" STREQUAL "${CMAKE_CUDA_COMPILER_ID}")
  set(option_init ON)
endif()

option(THRUST_ENABLE_TESTS_WITH_RDC
  "Build all Thrust tests with RDC; tests that require RDC are not affected by this option."
  ${option_init}
)

option(THRUST_ENABLE_EXAMPLES_WITH_RDC
  "Build all Thrust examples with RDC; examples which require RDC are not affected by this option."
  ${option_init}
)

# Check for RDC/SM compatibility and error/warn if necessary
foreach (sm IN LISTS no_rdc_archs)
  set(sm_opt THRUST_ENABLE_COMPUTE_${sm})
  if (${sm_opt})
    foreach (opt IN ITEMS TESTS EXAMPLES)
      set(rdc_opt THRUST_ENABLE_${opt}_WITH_RDC)
      if (${rdc_opt})
        message(FATAL_ERROR
          "${rdc_opt} is incompatible with ${sm_opt}, since sm_${sm} does not "
          "support RDC."
        )
      endif()
    endforeach()

    message(NOTICE
      "sm_${sm} does not support RDC. Targets that require RDC will be built "
      "without support for this architecture."
    )
  endif()
endforeach()

# By default RDC is not used:
set(CMAKE_CUDA_FLAGS "${THRUST_MUSA_FLAGS_BASE} ${THRUST_MUSA_FLAGS_NO_RDC}")

# MUSA: Set CUB_MUSA_ARCH for correct warp size calculation
# The architecture values are: mp21=210, mp22=220, mp31=310
if (DEFINED CMAKE_MUSA_COMPILER OR MUSA_FOUND)
  # Find the highest enabled architecture
  set(CUB_MUSA_ARCH_VALUE 0)
  set(enabled_archs "")
  foreach(arch IN LISTS THRUST_KNOWN_COMPUTE_ARCHS)
    if(THRUST_ENABLE_COMPUTE_${arch})
      list(APPEND enabled_archs ${arch})
      if(arch EQUAL 31)
        set(CUB_MUSA_ARCH_VALUE 310)
      elseif(arch EQUAL 22)
        set(CUB_MUSA_ARCH_VALUE 220)
      elseif(arch EQUAL 21)
        set(CUB_MUSA_ARCH_VALUE 210)
      endif()
    endif()
  endforeach()

  # Warn if multiple architectures are enabled
  list(LENGTH enabled_archs num_enabled)
  if(num_enabled GREATER 1)
    message(WARNING "Multiple MUSA architectures enabled: ${enabled_archs}. "
      "CUB_MUSA_ARCH will be set to the highest (${CUB_MUSA_ARCH_VALUE}). "
      "This may cause runtime issues on lower architectures. "
      "Consider enabling only one architecture.")
  endif()

  # Add the definition for both host and device code
  add_compile_definitions(CUB_MUSA_ARCH=${CUB_MUSA_ARCH_VALUE})
  # Also define MUSA_ARCH_LIST for namespace magic (must match CUB_MUSA_ARCH)
  add_compile_definitions(MUSA_ARCH_LIST=${CUB_MUSA_ARCH_VALUE})
  message(STATUS "Thrust: CUB_MUSA_ARCH set to ${CUB_MUSA_ARCH_VALUE}")
endif()
