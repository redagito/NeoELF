cmake_minimum_required (VERSION 3.16.0)
project (NeoELF)

set (VS_STARTUP_PROJECT ${PROJECT_NAME})

# Download automatically
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message (STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file (DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/master/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif ()
include (${CMAKE_BINARY_DIR}/conan.cmake)

set (CXX_STANDARD_REQUIRED ON)

# NeoELF library
add_subdirectory (Lib)

# Unit tests
add_subdirectory (Test)

# Engine application
add_subdirectory (App)
