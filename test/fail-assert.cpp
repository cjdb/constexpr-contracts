// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "cjdb/contracts.hpp"

int main(int argc, char**)
{
	CJDB_ASSERT(argc > 1);
	constexpr auto return_code = 255;
	return argc == 1 ? 0 : return_code;
}
