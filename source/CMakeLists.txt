# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0
#
cxx_library(
  TARGET contracts
  FILENAME "contracts.cpp"
  LIBRARY_TYPE STATIC
  INCLUDE_AND_EXPORT ${LLVM_INCLUDE_DIRS}
  LINK_AND_EXPORT ${llvm_libs}
)
