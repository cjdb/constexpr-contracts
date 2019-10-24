// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <cjdb/contracts.hpp>
#include <cstdio>
#include <cstdlib>

int main(int argc, char**)
{
	CJDB_EXPECTS(argc != 1);
	if (argc == 1) {
		std::fprintf(stderr, "This should never be printed.\n");
		std::exit(1);
	}

	constexpr auto return_code = 255;
	return return_code;
}
