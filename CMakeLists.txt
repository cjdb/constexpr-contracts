# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0
#
cmake_minimum_required(VERSION 3.17)

project("CONSTEXPR_CONTRACTS" C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "${PROJECT_TEMPLATE_CXX_COMPILER_MINIMUM_VERSION}")
	message(FATAL_ERROR "${PROJECT_NAME} requires C++ compiler ${CMAKE_CXX_COMPILER_ID} >=${PROJECT_TEMPLATE_CXX_COMPILER_MINIMUM_VERSION}, but found ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}.")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config/cmake")

# Project configuration
# Build

target_include_directories(contracts_consolation INTERFACE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)

enable_testing()
include(CTest)

option(${PROJECT_NAME}_ENABLE_CLANG_TIDY "Builds with clang-tidy, if available. Defaults to On." On)
set(${PROJECT_NAME}_CLANG_TIDY_PATH "/usr/bin/clang-tidy" CACHE FILEPATH "Path to clang-tidy. Defaults to /usr/bin/clang-tidy")
if(${PROJECT_NAME}_ENABLE_CLANG_TIDY)
	find_package(ClangTidy REQUIRED)
endif()

include(add_targets)
include(packages)

include_directories(include)
add_subdirectory(source)
add_subdirectory(test)

# Install

include(CMakePackageConfigHelpers)
install(TARGETS contracts_consolation EXPORT contracts_consolation-targets DESTINATION lib)
install(EXPORT contracts_consolation-targets FILE contracts_consolation-config.cmake
   DESTINATION lib/cmake/contracts_consolation)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*")
export(EXPORT contracts_consolation-targets FILE contracts_consolation-config.cmake)
