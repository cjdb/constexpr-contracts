# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
cmake_minimum_required(VERSION 3.13)
project("constexpr-contracts" CXX)

add_library(${PROJECT_NAME} INTERFACE)
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

target_include_directories(constexpr-contracts INTERFACE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)

enable_testing()
include(CTest)

add_subdirectory(test)

# Install

include(CMakePackageConfigHelpers)
install(TARGETS constexpr-contracts
        EXPORT  constexpr-contracts
        DESTINATION lib)
install(EXPORT constexpr-contracts
        FILE constexpr-contracts-config.cmake
        NAMESPACE cjdb::
        DESTINATION lib/cmake/constexpr-contracts)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*")
