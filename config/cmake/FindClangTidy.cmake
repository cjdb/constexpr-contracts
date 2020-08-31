# Copyright (c) Morris Hafner.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
include(FindPackageHandleStandardArgs)

if((NOT ${${PROJECT_NAME}_ENABLE_CLANG_TIDY}) OR (${PROJECT_NAME}_CLANG_TIDY_PATH STREQUAL ""))
   if(NOT ${${PROJECT_NAME}_ENABLE_CLANG_TIDY})
      message(STATUS "clang-tidy disabled.")
   else()
      message(STATUS "No path for clang-tidy.")
   endif()
   set(CMAKE_CXX_CLANG_TIDY "")
   return()
endif()

if(EXISTS "${${PROJECT_NAME}_CLANG_TIDY_PATH}")
   find_package(Clang REQUIRED)
   set(ClangTidy_FOUND On)
   set(CMAKE_CXX_CLANG_TIDY "${${PROJECT_NAME}_CLANG_TIDY_PATH}" -p=${CMAKE_BINARY_DIR})
   message(STATUS "Found ${${PROJECT_NAME}_CLANG_TIDY_PATH}")
else()
   set(ClangTidy_FOUND Off)
   message(STATUS "${PROJECT_NAME}_CLANG_TIDY_PATH=\"${${PROJECT_NAME}_CLANG_TIDY_PATH}\" not found.")
endif()

find_package_handle_standard_args(ClangTidy
   REQUIRED_VARS ClangTidy_FOUND)
