# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
cmake_minimum_required(VERSION 3.14)
project("contracts" CXX)

add_library(contracts_consolation INTERFACE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config/cmake")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure

option(ENABLE_CLANG_TIDY "Enables building with clang-tidy" Off)
set(CLANG_TIDY_PATH "/usr/bin/clang-tidy" CACHE
   STRING "Sets the path for clang-tidy. Defaults to \"/usr/bin/clang-tidy\".")

if(${ENABLE_CLANG_TIDY})
	set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_PATH}" "-p=${CMAKE_BINARY_DIR}")
endif()

# Build

target_include_directories(contracts_consolation INTERFACE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)

enable_testing()
include(CTest)

add_subdirectory(test)

# Install

include(CMakePackageConfigHelpers)
install(TARGETS contracts_consolation EXPORT contracts_consolation-targets DESTINATION lib)
install(EXPORT contracts_consolation-targets FILE contracts_consolation-config.cmake
   DESTINATION lib/cmake/contracts_consolation)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*")
export(EXPORT contracts_consolation-targets FILE contracts_consolation-config.cmake)
