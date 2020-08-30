# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.18)

project("TODO_RENAME_ME" CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "${PROJECT_TEMPLATE_CXX_COMPILER_MINIMUM_VERSION}")
	message(FATAL_ERROR "Project requires C++ compiler ${CMAKE_CXX_COMPILER_ID} >=${PROJECT_TEMPLATE_CXX_COMPILER_MINIMUM_VERSION}, but found ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}.")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config/cmake")

# Project configuration
enable_testing()
include(CTest)

include(add_targets)
include(packages)

include_directories(include)
add_subdirectory(source)
