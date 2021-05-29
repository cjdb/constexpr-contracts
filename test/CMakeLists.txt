# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0
#
set(${PROJECT_NAME}_TEST_FRAMEWORK "Catch2::Catch2" CACHE STRING "")
set(${PROJECT_NAME}_NEEDS_TEST_MAIN ON CACHE BOOL "")

cxx_library(
   TARGET test_main
   FILENAME "test_main.cpp"
   LINK_TARGETS Catch2::Catch2
)

cxx_test(
   TARGET test_greeting
   FILENAME "greeting_test.cpp"
   LINK_TARGETS greeting
)
