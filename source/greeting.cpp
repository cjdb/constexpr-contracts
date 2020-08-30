// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0
//
#include <project_template/greeting.hpp>
#include <string>
#include <string_view>

namespace project_template {
	std::string greet(std::string_view name)
	{
		return std::string("Hello, ").append(name, '!');
	}
} // namespace project_template
