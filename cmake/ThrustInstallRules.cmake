# Bring in CMAKE_INSTALL_LIBDIR
include(GNUInstallDirs)

# MUSA toolkits place CMake package files under ${prefix}/lib, not lib64.
set(thrust_cmake_install_dir "lib/cmake/thrust")
set(cub_cmake_install_dir "lib/cmake/cub")

# Thrust is a header library; no need to build anything before installing:
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY TRUE)

install(DIRECTORY "${Thrust_SOURCE_DIR}/thrust"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.inl"
)
if (EXISTS "${Thrust_SOURCE_DIR}/thrust/statement")
  install(FILES "${Thrust_SOURCE_DIR}/thrust/statement"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/thrust")
endif()

install(DIRECTORY "${Thrust_SOURCE_DIR}/thrust/cmake/"
  DESTINATION "${thrust_cmake_install_dir}"
  PATTERN thrust-header-search EXCLUDE
)
# Need to configure a file to store the infix specified in
# CMAKE_INSTALL_INCLUDEDIR since it can be defined by the user
set(install_location "${thrust_cmake_install_dir}")
configure_file("${Thrust_SOURCE_DIR}/thrust/cmake/thrust-header-search.cmake.in"
  "${Thrust_BINARY_DIR}/thrust/cmake/thrust-header-search.cmake"
  @ONLY)
install(FILES "${Thrust_BINARY_DIR}/thrust/cmake/thrust-header-search.cmake"
  DESTINATION "${install_location}")

# Depending on how Thrust is configured, CUB's CMake scripts may or may not be
# included, so maintain a set of CUB install rules in both projects. By default
# CUB headers are installed alongside Thrust -- this may be disabled by turning
# off THRUST_INSTALL_CUB_HEADERS.
set(thrust_install_cub_default OFF)
if (THRUST_MUSA_FOUND)
  set(thrust_install_cub_default ON)
endif()
option(THRUST_INSTALL_CUB_HEADERS "Include cub headers when installing." ${thrust_install_cub_default})
if (THRUST_INSTALL_CUB_HEADERS)
  install(DIRECTORY "${Thrust_SOURCE_DIR}/dependencies/cub/cub"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING
      PATTERN "*.cuh"
  )
  if (EXISTS "${Thrust_SOURCE_DIR}/dependencies/cub/cub/statement")
    install(FILES "${Thrust_SOURCE_DIR}/dependencies/cub/cub/statement"
      DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/cub")
  endif()

  # Need to configure a file to store THRUST_INSTALL_HEADER_INFIX
  install(DIRECTORY "${Thrust_SOURCE_DIR}/dependencies/cub/cub/cmake/"
    DESTINATION "${cub_cmake_install_dir}"
    PATTERN cub-header-search EXCLUDE
  )
  set(install_location "${cub_cmake_install_dir}")
  configure_file("${Thrust_SOURCE_DIR}/dependencies/cub/cub/cmake/cub-header-search.cmake.in"
    "${Thrust_BINARY_DIR}/dependencies/cub/cub/cmake/cub-header-search.cmake"
    @ONLY)
  install(FILES "${Thrust_BINARY_DIR}/dependencies/cub/cub/cmake/cub-header-search.cmake"
    DESTINATION "${install_location}")
endif()
