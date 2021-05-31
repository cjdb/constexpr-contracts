// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#ifndef CJDB_CONTRACTS_HPP
#define CJDB_CONTRACTS_HPP

#include <string_view>
#include <type_traits>

#ifdef _MSC_VER
	#define CJDB_PRETTY_FUNCTION __FUNCSIG__
	#define CJDB_FORCE_INLINE __forceinline
#else
	#define CJDB_PRETTY_FUNCTION __PRETTY_FUNCTION__
	#define CJDB_FORCE_INLINE [[gnu::always_inline]] inline
#endif // _MSC_VER

#ifndef CJDB_PRINT_ERROR
	#ifdef CJDB_USE_STDIO
		#include <cstdio>

		namespace cjdb::contracts_detail {
			struct print_error_fn {
				CJDB_FORCE_INLINE void operator()(std::string_view message) const noexcept
				{
					std::fwrite(message.data(), sizeof(char), message.size(), stderr);
				}
			};
			inline constexpr auto print_error = print_error_fn{};
		} // namespace cjdb::contracts_detail
	#else
		#include <iostream>

		namespace cjdb::contracts_detail {
			struct print_error_fn {
				CJDB_FORCE_INLINE void operator()(std::string_view message) const noexcept
				try {
					std::cerr.write(message.data(),
					                static_cast<std::streamsize>(message.size()));
				} catch(...) {}
			};
			inline constexpr auto print_error = print_error_fn{};
		} // namespace cjdb::contracts_detail
	#endif // CJDB_USE_STDIO
	#define CJDB_PRINT_ERROR(MESSAGE) ::cjdb::contracts_detail::print_error(MESSAGE)
#endif // CJDB_PRINT_ERROR

// clang-tidy doesn't yet support this
//
// #ifndef __cpp_lib_is_constant_evaluated
// #  error "Contracts Consolation requires a compiler supporting std::is_constant_evaluated."
// #endif // __cpp_lib_is_constant_evaluated

#define CJDB_EXPECTS(...) CJDB_CONTRACT_IMPL("pre-condition", __VA_ARGS__)
#define CJDB_ASSERT(...)  CJDB_CONTRACT_IMPL("assertion", __VA_ARGS__)
#define CJDB_ENSURES(...) CJDB_CONTRACT_IMPL("post-condition", __VA_ARGS__)

namespace cjdb::contracts_detail {
	#ifdef NDEBUG
		inline constexpr auto is_debug = false;
	#else
		inline constexpr auto is_debug = true;
	#endif // NDEBUG

	struct contract_impl_fn {
		constexpr void operator()(bool const result,
		                          std::string_view const message,
		                          std::string_view const function) const noexcept
		{
			if (not result) {
				if (not std::is_constant_evaluated()) {
					if constexpr (is_debug) {
						CJDB_PRINT_ERROR(message);
						CJDB_PRINT_ERROR(function);
						CJDB_PRINT_ERROR("`\n");
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
	};
	inline constexpr auto contract_impl = contract_impl_fn{};

	// This function doesn't use std::same_as, since the library is used by cjdb-ranges, which is
	// an implementation of standard concepts.
	struct matches_bool_fn {
		template<typename T>
		requires std::is_same_v<std::remove_cvref_t<T>, bool>
		constexpr bool operator()(T const t) const noexcept
		{
			return t;
		}
	};
	inline constexpr auto matches_bool = matches_bool_fn{};
} // namespace cjdb::contracts_detail

#define CJDB_CONTRACT_IMPL(CJDB_KIND, ...) \
   ::cjdb::contracts_detail::contract_impl(::cjdb::contracts_detail::matches_bool(__VA_ARGS__), \
      __FILE__ ":" CJDB_TO_STRING(__LINE__) ": " CJDB_KIND " `" #__VA_ARGS__ "` failed in `", \
      CJDB_PRETTY_FUNCTION)


#define CJDB_TO_STRING(CJDB_STRING) CJDB_TO_STRING_IMPL(CJDB_STRING)
#define CJDB_TO_STRING_IMPL(CJDB_STRING) #CJDB_STRING

#endif // CJDB_CONTRACTS_HPP
