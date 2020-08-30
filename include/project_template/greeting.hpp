// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0
//
#ifndef PROJECT_TEMPLATE_GREETING_HPP
#define PROJECT_TEMPLATE_GREETING_HPP

#include <string>
#include <string_view>

namespace project_template {
	// Given a name, returns a friendly message.
	std::string greet(std::string_view name);
} // namespace project_template

#endif // PROJECT_TEMPLATE_GREETING_HPP
