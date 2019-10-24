// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#ifndef CJDB_COCO_TEST_HPP
#define CJDB_COCO_TEST_HPP

#include "cjdb/contracts.hpp"

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>

namespace cjdb_test {
	template<typename T>
	// requires regular<T>
	constexpr bool evaluate_contract(T const value)
	{
		CJDB_EXPECTS(value == 'a');

		auto const b = value + 1;
		CJDB_ASSERT(b != 'a');
		CJDB_ASSERT(b == 'b');

		auto const e = b + 3;
		return CJDB_ENSURES(e == 'e'), true;
	}

	inline bool evaluate_contract(std::vector<int>& v)
	{
		CJDB_EXPECTS(std::empty(v));

		v.emplace_back(100);
		CJDB_ASSERT(not std::empty(v));

		return CJDB_ENSURES(v[0] == 100), true;
	}
} // namespace cjdb_test

#define LIGHT_CHECK(...) {                                                                   \
	if (not bool((__VA_ARGS__))) {                                                            \
		std::cerr << "Expression `" #__VA_ARGS__ "` evaluated as false at " << __FILE__ << ":" \
		          << __LINE__ << '\n';                                                         \
		std::abort();                                                                          \
	}                                                                                         \
}


#define CJDB_CONSTEXPR_CHECK(...) {                   \
	static_assert(std::bool_constant<__VA_ARGS__>{});  \
	LIGHT_CHECK(__VA_ARGS__);                          \
}

#endif // CJDB_COCO_TEST_HPP
