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
string(JOIN " " CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" "${CONSTEXPR_CONTRACTS_FLAGS_DEBUG}")
string(JOIN " " CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" "${CONSTEXPR_CONTRACTS_FLAGS_RELEASE}")

# Build

enable_testing()
include(CTest)

option(${PROJECT_NAME}_ENABLE_CLANG_TIDY "Builds with clang-tidy, if available. Defaults to Off." Off)
set(${PROJECT_NAME}_CLANG_TIDY_PATH "/usr/bin/clang-tidy" CACHE FILEPATH "Path to clang-tidy. Defaults to /usr/bin/clang-tidy")
if(${PROJECT_NAME}_ENABLE_CLANG_TIDY)
	find_package(ClangTidy REQUIRED)
endif()

include(add_targets)
include(packages)

add_subdirectory(source)
add_subdirectory(test)

# Install

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(TARGETS contracts
        EXPORT  contracts
        DESTINATION lib)
install(EXPORT contracts
        FILE constexpr-contracts-config.cmake
        NAMESPACE cjdb::
        DESTINATION lib/cmake/constexpr-contracts)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*")
