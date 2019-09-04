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
#include <string_view>
#include <type_traits>

// clang-tidy doesn't yet support this
//
// #ifndef __cpp_lib_is_constant_evaluated
// #  error "Contracts Consolation requires a compiler supporting std::is_constant_evaluated."
// #endif // __cpp_lib_is_constant_evaluated

#define CJDB_EXPECTS(...) CJDB_CONTRACT_IMPL("pre-condition", __VA_ARGS__)
#define CJDB_ASSERT(...)  CJDB_CONTRACT_IMPL("assertion", __VA_ARGS__)
#define CJDB_ENSURES(...) CJDB_CONTRACT_IMPL("post-condition", __VA_ARGS__)

#ifdef _MSC_VER
	#define CJDB_PRETTY_FUNCTION __FUNCSIG__
#else
	#define CJDB_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif // _MSC_VER

namespace cjdb::contracts_detail {
	#ifdef NDEBUG
		inline constexpr auto is_debug = false;
	#else
		inline constexpr auto is_debug = true;
	#endif // NDEBUG

	constexpr void contract_impl(bool const result,
	                             std::string_view const message,
	                             std::string_view const function) noexcept
	{
		if (not result) {
			if (not std::is_constant_evaluated()) {
				if constexpr (is_debug) {
					std::fprintf(stderr, "%s in `%s`\n", message.data(), function.data());
				}
			}
		#ifdef _MSC_VER
			__assume(false);
		#elif defined(__OPTIMIZE__)
			__builtin_unreachable();
		#else
			__builtin_trap();
		#endif // __OPTIMIZE__
		}
	}
} // namespace cjdb::contracts_detail

#define CJDB_CONTRACT_IMPL(CJDB_KIND, ...) \
   ::cjdb::contracts_detail::contract_impl(static_cast<bool>(__VA_ARGS__),    \
      __FILE__ ":" CJDB_TO_STRING(__LINE__) ": " CJDB_KIND " `" #__VA_ARGS__ "` failed", \
      CJDB_PRETTY_FUNCTION)


#define CJDB_TO_STRING(CJDB_STRING) CJDB_TO_STRING_IMPL(CJDB_STRING)
#define CJDB_TO_STRING_IMPL(CJDB_STRING) #CJDB_STRING

#endif // CJDB_CONTRACTS_HPP
