# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
cmake_minimum_required(VERSION 3.14)
project(PackageTest CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(example example.cpp)
target_include_directories(example PRIVATE ../include)
target_link_libraries(example ${CONAN_LIBS})

enable_testing()
add_test(NAME example
         WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
         COMMAND example)
