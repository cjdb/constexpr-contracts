// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <iostream>
#include "cjdb/contracts.hpp"

constexpr int check_constexpr_contract(int x) noexcept
{
	CJDB_EXPECTS(x == 0);

	++x;
	CJDB_ASSERT(x == 1);

	--x;
	return CJDB_ENSURES(x == 0), x;
}

int main() {
	[[maybe_unused]] constexpr auto x = check_constexpr_contract(0);

	auto y = 0;
	return check_constexpr_contract(y);
}
