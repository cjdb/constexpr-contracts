
// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#ifndef CJDB_CONTRACTS_HPP
#define CJDB_CONTRACTS_HPP

#include <cstring>
#include <string_view>
#include <type_traits>

#ifdef CJDB_USE_IOSTREAM
	#include <iostream>
#elif !defined(CJDB_SKIP_STDIO)
	#include <cerrno>
	#include <cstdio>
	#include <system_error>
#endif // CJDB_USE_IOSTREAM

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

namespace cjdb {
	using print_error_fn = void(std::string_view);
	inline print_error_fn* print_error = [](std::string_view message) {
#ifdef CJDB_USE_IOSTREAM
		std::cerr.write(message.data(), static_cast<std::streamsize>(message.size()));
#elif !defined(CJDB_SKIP_STDIO)
		if (auto const len = message.size();
			std::fwrite(message.data(), sizeof(char), len, stderr) < len) [[unlikely]]
		{
			throw std::system_error{errno, std::system_category()};
		}
#endif // CJDB_USE_IOSTREAM
	};

namespace contracts_detail {
	#ifdef NDEBUG
		inline constexpr auto is_debug = false;
	#else
		inline constexpr auto is_debug = true;
	#endif // NDEBUG

	struct contract_impl_fn {
		template<std::size_t N1, std::size_t N2>
		constexpr void operator()(bool const result,
		                          char const(&message)[N1], // NOLINT(modernize-avoid-c-arrays)
		                          char const(&function)[N2]) const noexcept // NOLINT(modernize-avoid-c-arrays)
		{
			if (not result) {
				if (not std::is_constant_evaluated()) {
					if constexpr (is_debug) { // NOLINT
					#ifdef _WIN32
						constexpr auto& suffix = "`\r\n";
					#else
						constexpr auto& suffix = "`\n";
					#endif // _WIN32
						constexpr auto message_size = N1 - 1;
						constexpr auto function_size = N2 - 1;
						// NOLINTNEXTLINE(modernize-avoid-c-arrays)
						char full_message[message_size + function_size + sizeof suffix]{};
						auto p = full_message;
						std::memcpy(p, message, message_size);
						std::memcpy(p += message_size, function, function_size);
						std::memcpy(p += function_size, suffix, sizeof suffix - 1);
						::cjdb::print_error(full_message);
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
} // namespace contracts_detail
} // namespace cjdb

#define CJDB_CONTRACT_IMPL(CJDB_KIND, ...) \
   ::cjdb::contracts_detail::contract_impl(::cjdb::contracts_detail::matches_bool(__VA_ARGS__), \
      __FILE__ ":" CJDB_TO_STRING(__LINE__) ": " CJDB_KIND " `" #__VA_ARGS__ "` failed in `", \
      CJDB_PRETTY_FUNCTION)


#define CJDB_TO_STRING(CJDB_STRING) CJDB_TO_STRING_IMPL(CJDB_STRING)
#define CJDB_TO_STRING_IMPL(CJDB_STRING) #CJDB_STRING

#endif // CJDB_CONTRACTS_HPP
