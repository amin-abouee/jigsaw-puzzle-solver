cmake_minimum_required(VERSION 2.8)
project(JigsawPuzzleSolver)

SET(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake/ )

# set compiler for using of default gcc via macport (gcc 4.8)
# set(CMAKE_CXX_COMPILER "/opt/local/bin/g++")
# set(CMAKE_C_COMPILER "/opt/local/bin/gcc")

# set the release mode for build type
set(CMAKE_BUILD_TYPE "Debug")

# set c++ compiler flags -on warning and using of new c++ strandard library ISO 2011 (-std=c++0x)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

# message for set the compiler and its flags and type of build(release)
message ("user define: CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
message ("user define: CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")
message ("user define: CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")


if(CMAKE_BUILD_TYPE STREQUAL Debug)
	set (BUILD_PREFIX "-d")
else()
	set (BUILD_PREFIX "")
endif()

add_subdirectory(src)
