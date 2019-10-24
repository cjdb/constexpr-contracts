// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "cjdb/contracts.hpp"

#include "coco_test.hpp"
#include <type_traits>
#include <vector>

int main()
{
	CJDB_CONSTEXPR_CHECK(cjdb_test::evaluate_contract(static_cast<int>('a')));
	CJDB_CONSTEXPR_CHECK(cjdb_test::evaluate_contract(static_cast<double>('a')));
	CJDB_CONSTEXPR_CHECK(cjdb_test::evaluate_contract('a'));

	auto v = std::vector<int>{};
	LIGHT_CHECK(cjdb_test::evaluate_contract(v));
}
