set(MUSA_INSTALL_DIR "/usr/local/musa" CACHE STRING "set musa install directory")
list(APPEND CMAKE_MODULE_PATH ${MUSA_INSTALL_DIR}/cmake)
find_package(MUSA REQUIRED)