# Copyright (c) Morris Hafner.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
include(FindPackageHandleStandardArgs)

if(NOT "${${PROJECT_NAME}_ENABLE_CLANG_TIDY}")
   message(STATUS "clang-tidy disabled.")
   set(CMAKE_CXX_CLANG_TIDY "")
   return()
endif()

set(clang_tidy_path "${${PROJECT_NAME}_CLANG_TIDY_PATH}")

if(EXISTS "${clang_tidy_path}")
   find_package(Clang REQUIRED)
   set(ClangTidy_FOUND On)
   set(CMAKE_CXX_CLANG_TIDY "${clang_tidy_path}" -p=${CMAKE_BINARY_DIR})
   message(STATUS "Found ${clang_tidy_path}")
else()
   set(ClangTidy_FOUND Off)
   message(STATUS "${PROJECT_NAME}_CLANG_TIDY_PATH=\"${clang_tidy_path}\" not found.")
endif()

find_package_handle_standard_args(ClangTidy REQUIRED_VARS ClangTidy_FOUND)
