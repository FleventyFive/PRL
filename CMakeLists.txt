cmake_minimum_required(VERSION 3.1)

project(Roguelike)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

include_directories(src)
include_directories(src/components)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "Building in the source directory is not supported, please build in a build directory")
endif()

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++17 -stdlib=libc++")
set(CMAKE_SKIP_RPATH TRUE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)


find_program(CLANGTIDY clang-tidy)
if(CLANGTIDY)
	file(GLOB srcs
		${CMAKE_SOURCE_DIR}/src/*.cpp
		${CMAKE_SOURCE_DIR}/src/*.hpp
		${CMAKE_SOURCE_DIR}/src/components/*.cpp
		${CMAKE_SOURCE_DIR}/src/components/*.hpp
	)
	add_custom_target(lint
		clang-tidy
		-checks='*'
		${srcs}
		--
		-std=c++17
	)
endif()