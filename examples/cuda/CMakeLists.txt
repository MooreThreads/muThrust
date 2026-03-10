file(GLOB example_srcs
  RELATIVE "${CMAKE_CURRENT_LIST_DIR}}"
  CONFIGURE_DEPENDS
  *.cu *.cpp
)

# MUSA: Examples that require CUDA Dynamic Parallelism (CDP) are disabled
# because MUSA does not support RDC (Relocatable Device Code)
set(MUSA_DISABLED_EXAMPLES
  async_reduce  # Requires CDP: calls thrust::reduce from __global__ kernel
)

foreach(thrust_target IN LISTS THRUST_TARGETS)
  thrust_get_target_property(config_device ${thrust_target} DEVICE)
  if (NOT config_device STREQUAL "CUDA")
    continue()
  endif()

  foreach(example_src IN LISTS example_srcs)
    get_filename_component(example_name "${example_src}" NAME_WLE)

    # MUSA: Skip examples that require CDP
    if(${example_name} IN_LIST MUSA_DISABLED_EXAMPLES)
      message(STATUS "MUSA: Skipping example cuda.${example_name} (requires CUDA Dynamic Parallelism)")
      continue()
    endif()

    string(PREPEND example_name "cuda.")
    thrust_add_example(example_target ${example_name} "${example_src}" ${thrust_target})
  endforeach()
endforeach()
