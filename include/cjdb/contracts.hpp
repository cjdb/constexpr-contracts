//
//  Copyright Christopher Di Bella
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef CJDB_CONTRACTS_HPP
#define CJDB_CONTRACTS_HPP

#include <cstdio>
#include <cstdlib>
#include <string_view>
#include <type_traits>

// clang-tidy doesn't yet support this
//
// #ifndef __cpp_lib_is_constant_evaluated
// #  error "Contracts Consolation requires a compiler supporting std::is_constant_evaluated."
// #endif // __cpp_lib_is_constant_evaluated

#define CJDB_QUIET_CONTRACT 0
#define CJDB_ALERTING_CONTRACT 1

#define CJDB_QUIET_EXPECTS(...) CJDB_EXPECTS_IMPL(CJDB_QUIET_CONTRACT, __VA_ARGS__)
#define CJDB_ALERT_EXPECTS(...) CJDB_EXPECTS_IMPL(CJDB_ALERTING_CONTRACT, __VA_ARGS__)

#define CJDB_QUIET_ASSERT(...) CJDB_ASSERT_IMPL(CJDB_QUIET_CONTRACT, __VA_ARGS__)
#define CJDB_ALERT_ASSERT(...) CJDB_ASSERT_IMPL(CJDB_ALERTING_CONTRACT, __VA_ARGS__)

#define CJDB_QUIET_ENSURES(...) CJDB_ENSURES_IMPL(CJDB_QUIET_CONTRACT, __VA_ARGS__)
#define CJDB_ALERT_ENSURES(...) CJDB_ENSURES_IMPL(CJDB_ALERTING_CONTRACT, __VA_ARGS__)

#define CJDB_EXPECTS_IMPL(CJDB_CONTRACT_MODE, ...) \
	CJDB_CONTRACT_IMPL("pre-condition", CJDB_CONTRACT_MODE, __VA_ARGS__)

#define CJDB_ASSERT_IMPL(CJDB_CONTRACT_MODE, ...) \
	CJDB_CONTRACT_IMPL("assertion", CJDB_CONTRACT_MODE, __VA_ARGS__)

#define CJDB_ENSURES_IMPL(CJDB_CONTRACT_MODE, ...) \
	CJDB_CONTRACT_IMPL("post-condition", CJDB_CONTRACT_MODE, __VA_ARGS__)

#ifdef NDEBUG
	#define CJDB_IS_DEBUG false
#else
	#define CJDB_IS_DEBUG true
#endif // NDEBUG

#define CJDB_CONTRACT_IMPL(CJDB_KIND, CJDB_CONTRACT_MODE, ...) (void)([](bool const result) constexpr {                                      \
	auto fail = [](bool const result) consteval noexcept {                                                                                    \
		if (result) {                                                                                                                          \
			return true;                                                                                                                        \
		}                                                                                                                                      \
		std::abort();                                                                                                                          \
	};                                                                                                                                        \
	if (__builtin_expect(not result, false)) {                                                                                                \
		if (std::is_constant_evaluated()) {                                                                                                    \
			fail(false);                                                                                                                        \
		}                                                                                                                                      \
		return false;                                                                                                                          \
	}                                                                                                                                         \
    return true;                                                                                                                             \
}(static_cast<bool>(__VA_ARGS__)) ? (void)0 : [](std::string_view const function) {                                                          \
	if (CJDB_IS_DEBUG or CJDB_CONTRACT_MODE == CJDB_ALERTING_CONTRACT) {                                                                      \
		std::fprintf(stderr, CJDB_KIND " `" #__VA_ARGS__ "` failed in `%s` at " __FILE__ ":" CJDB_TO_STRING(__LINE__) "\n", function.data());  \
	}                                                                                                                                         \
	std::abort();                                                                                                                             \
}(__PRETTY_FUNCTION__))


#define CJDB_TO_STRING(CJDB_STRING) CJDB_TO_STRING_IMPL(CJDB_STRING)
#define CJDB_TO_STRING_IMPL(CJDB_STRING) #CJDB_STRING

#define CJDB_AXIOM(_) true

#endif // CJDB_CONTRACTS_HPP
