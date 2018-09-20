cmake_minimum_required(VERSION 3.1)

project(Roguelike C CXX)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "Building in the source directory is not supported, please build in a build directory")
endif()

# Add custom CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules/")

# We require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++17 -stdlib=libc++")
set(CMAKE_SKIP_RPATH TRUE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find and include required packages
find_package(BearLibTerminal REQUIRED)
include_directories(${BLT_INCLUDE_DIRS})

add_subdirectory(src)

if(${WIN32})
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()
set(CPACK_PACKAGE_NAME "bearlibTest")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CPACK_SYSTEM_NAME "OSX-${EX_PLATFORM_NAME}-${CMAKE_CXX_COMPILER_ID}")
else()
    set(CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${EX_PLATFORM_NAME}-${CMAKE_CXX_COMPILER_ID}")
endif()
include(CPack)
