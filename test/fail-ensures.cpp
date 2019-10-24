// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "cjdb/contracts.hpp"
#include <cstdio>
#include <cstdlib>

int magic(int x) noexcept
{
	return CJDB_ENSURES(x > 1), x * 2;
}

int main(int const argc, char**)
{
	auto const x = magic(argc);
	if (x == 2) {
		std::fprintf(stderr, "This should never be printed.\n");
		std::exit(1);
	}

	constexpr auto return_code = 255;
	return return_code;
}
