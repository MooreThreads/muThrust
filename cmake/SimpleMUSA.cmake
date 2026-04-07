# SimpleMUSA.cmake
# Minimal MUSA configuration without external dependencies

# Set MUSA paths
set(MUSA_ROOT "/usr/local/musa" CACHE PATH "MUSA toolkit root directory")
set(MUSA_INCLUDE_DIR "${MUSA_ROOT}/include")
set(MUSA_LIBRARY_DIR "${MUSA_ROOT}/lib")

# Find MUSA compiler (mcc)
find_program(MCC_EXECUTABLE
    NAMES mcc
    PATHS "${MUSA_ROOT}/bin"
    ENV MUSA_PATH
    ENV MUSA_BIN_PATH
    PATH_SUFFIXES bin
)

if(NOT MCC_EXECUTABLE)
    message(FATAL_ERROR "MUSA compiler (mcc) not found. Set MUSA_ROOT or MUSA_PATH.")
endif()

message(STATUS "Found MUSA compiler: ${MCC_EXECUTABLE}")

# Set MUSA_FOUND so ThrustCudaConfig.cmake can detect MUSA
set(MUSA_FOUND TRUE CACHE INTERNAL "MUSA found")

# Find MUSA runtime library
find_library(MUSA_LIBRARY
    NAMES musart musa
    PATHS "${MUSA_LIBRARY_DIR}"
    ENV MUSA_PATH
    ENV MUSA_LIB_PATH
    PATH_SUFFIXES lib
)

if(NOT MUSA_LIBRARY)
    message(FATAL_ERROR "MUSA runtime library not found in ${MUSA_LIBRARY_DIR}")
endif()

message(STATUS "Found MUSA library: ${MUSA_LIBRARY}")

# Set C++ compiler to mcc
set(CMAKE_CXX_COMPILER "${MCC_EXECUTABLE}" CACHE FILEPATH "C++ compiler" FORCE)

# Add MUSA path to compile options globally
add_compile_options(--musa-path=${MUSA_ROOT})

# Set default MUSA architecture if not specified
if(NOT DEFINED MUSA_ARCH AND NOT DEFINED ENV{MUSA_ARCH})
    set(MUSA_ARCH "mp_31" CACHE STRING "MUSA target architecture")
elseif(DEFINED ENV{MUSA_ARCH})
    set(MUSA_ARCH "$ENV{MUSA_ARCH}" CACHE STRING "MUSA target architecture")
endif()

# Extract architecture number (mp_31 -> 31)
string(REGEX REPLACE "mp_" "" ARCH_NUM "${MUSA_ARCH}")

# Add -x musa immediately after compiler, then offload-arch
add_compile_options(-x musa --offload-arch=${MUSA_ARCH})

message(STATUS "MUSA target architecture: ${MUSA_ARCH}")

# Set MUSA compile flags for .cu files
set(CMAKE_CXX_COMPILE_OPTIONS_CREATE_PCH "-x" "musa" "${CMAKE_CXX_COMPILE_OPTIONS_CREATE_PCH}")

# Function to build MUSA executables
function(musa_add_executable target_name)
    # Parse arguments
    set(sources)
    set(options)
    set(cmake_options)
    set(found_options FALSE)

    foreach(arg ${ARGN})
        if("x${arg}" STREQUAL "xOPTIONS")
            set(found_options TRUE)
        elseif("x${arg}" MATCHES "^x(WIN32|MACOSX_BUNDLE|EXCLUDE_FROM_ALL)$")
            list(APPEND cmake_options ${arg})
        else()
            if(found_options)
                list(APPEND options ${arg})
            else()
                list(APPEND sources ${arg})
            endif()
        endif()
    endforeach()

    # Separate .cu files from other sources
    set(cu_sources)
    set(other_sources)
    foreach(src ${sources})
        get_filename_component(ext "${src}" EXT)
        if("${ext}" STREQUAL ".cu")
            list(APPEND cu_sources "${src}")
        else()
            list(APPEND other_sources "${src}")
        endif()
    endforeach()

    # Set .cu files to compile as CXX with -x musa
    if(cu_sources)
        set_source_files_properties(${cu_sources} PROPERTIES
            LANGUAGE CXX
            COMPILE_OPTIONS "-x;musa"
        )
    endif()

    # Create executable
    add_executable(${target_name} ${cmake_options} ${cu_sources} ${other_sources})

    # Apply MUSA compile options (use project's MUSA_MCC_FLAGS and MUSA_ARCH_FLAGS)
    if(DEFINED MUSA_MCC_FLAGS)
        target_compile_options(${target_name} PRIVATE ${MUSA_MCC_FLAGS})
    endif()
    if(DEFINED MUSA_ARCH_FLAGS)
        target_compile_options(${target_name} PRIVATE ${MUSA_ARCH_FLAGS})
    endif()
    if(options)
        target_compile_options(${target_name} PRIVATE ${options})
    endif()

    # Link MUSA runtime (no keyword to match existing code)
    target_link_libraries(${target_name} ${MUSA_LIBRARY})

    # Include MUSA headers (use include_directories for directory-level)
    include_directories(${MUSA_INCLUDE_DIR})
endfunction()

# Function to build MUSA libraries
function(musa_add_library target_name)
    # Parse arguments
    set(sources)
    set(options)
    set(cmake_options)
    set(found_options FALSE)

    foreach(arg ${ARGN})
        if("x${arg}" STREQUAL "xOPTIONS")
            set(found_options TRUE)
        elseif("x${arg}" MATCHES "^x(STATIC|SHARED|MODULE|OBJECT)$")
            list(APPEND cmake_options ${arg})
        else()
            if(found_options)
                list(APPEND options ${arg})
            else()
                list(APPEND sources ${arg})
            endif()
        endif()
    endforeach()

    # Separate .cu files from other sources
    set(cu_sources)
    set(other_sources)
    foreach(src ${sources})
        get_filename_component(ext "${src}" EXT)
        if("${ext}" STREQUAL ".cu")
            list(APPEND cu_sources "${src}")
        else()
            list(APPEND other_sources "${src}")
        endif()
    endforeach()

    # Set .cu files to compile as CXX with -x musa
    if(cu_sources)
        set_source_files_properties(${cu_sources} PROPERTIES
            LANGUAGE CXX
            COMPILE_OPTIONS "-x;musa"
        )
    endif()

    # Create library
    add_library(${target_name} ${cmake_options} ${cu_sources} ${other_sources})

    # Apply MUSA compile options (use project's MUSA_MCC_FLAGS and MUSA_ARCH_FLAGS)
    if(DEFINED MUSA_MCC_FLAGS)
        target_compile_options(${target_name} ${MUSA_MCC_FLAGS})
    endif()
    if(DEFINED MUSA_ARCH_FLAGS)
        target_compile_options(${target_name} ${MUSA_ARCH_FLAGS})
    endif()
    if(options)
        target_compile_options(${target_name} ${options})
    endif()

    # Link MUSA runtime (no keyword to match existing code)
    target_link_libraries(${target_name} ${MUSA_LIBRARY})

    # Include MUSA headers (use include_directories for directory-level)
    include_directories(${MUSA_INCLUDE_DIR})
endfunction()
