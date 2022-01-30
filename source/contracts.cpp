// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0
//

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cxxabi.h>
#include <execinfo.h>
#include <memory>
#include <ranges>
#include <span>
#include <string>
#include <string_view>

namespace cjdb::constexpr_contracts_detail {
	// TODO(cjdb): remove once Clang supports std::ranges::find
	template<class I, class S>
	[[nodiscard]] I find(I first, S last, char const value)
	{
		for (; *first != last; ++first) {
			if (*first == value) {
				return first;
			}
		}

		return first;
	}

	template<class I, class S, class Pred>
	[[nodiscard]] I find_if(I first, S last, Pred pred)
	{
		for (; *first != last; ++first) {
			if (pred(*first)) {
				return first;
			}
		}

		return first;
	}

	template<class I, class S, class Pred>
	I find_if_not(I first, S last, Pred pred)
	{
		for (; *first != last; ++first) {
			if (not pred(*first)) {
				return first;
			}
		}

		return first;
	}

	void print_stacktrace() noexcept
	{
		(void)fprintf(
		  stderr,
		  "\n------------------------------------------ Stack trace ------------------------------------------\n");

		auto stacktrace = std::array<void*, 64>{};
		auto const stacktrace_size = backtrace(stacktrace.data(), stacktrace.size());

		if (stacktrace_size == 0) {
			(void)fprintf(
			  stderr,
			  "<Unable to retrieve the stacktrace: you might have a corrupt stack, or debugging symbols "
			  "might not be enabled.>\n");
			return;
		}

		auto const symbol_data =
		  std::unique_ptr<char*, void (*)(void*)>(backtrace_symbols(stacktrace.data(), stacktrace_size), ::free);
		constexpr auto drop = 5;
		auto const symbols = std::span(symbol_data.get() + drop, static_cast<std::size_t>(stacktrace_size - drop));

		for (auto const symbol : symbols) {
			auto first = symbol;
			auto last = find(first, std::unreachable_sentinel, '(');
			auto const binary_name = std::string(first, last);

			first = last + 1;
			last = find_if_not(first, std::unreachable_sentinel, [](char const c) {
				return static_cast<bool>(std::isalnum(c)) or c == '_';
			});
			auto const mangled_symbol_name = std::string(first, last);

			first = find_if(last + 1, std::unreachable_sentinel, ::isdigit);
			last = find_if_not(first + 1, std::unreachable_sentinel, ::isalnum);
			auto const offset = std::string(first, last);

			std::size_t length = 0;
			int status = 0;
			auto const demangled_name = std::unique_ptr<char, void (*)(void*)>(
			  abi::__cxa_demangle(std::string(mangled_symbol_name).data(), nullptr, &length, &status),
			  ::free);

			if (status == 0) {
				(void)fprintf(stderr, "  %s:\t%s\n", binary_name.data(), demangled_name.get());
				continue;
			}

			if (not mangled_symbol_name.empty()) {
				(void)fprintf(stderr, "  %s:\t%s\n", binary_name.data(), mangled_symbol_name.data());
				continue;
			}

			(void)fprintf(stderr, "  %s:\t[%s]\n", binary_name.data(), offset.data());
		}
	}

	void print_contract_violation(signed char const value) noexcept
	{
		(void)fprintf(stderr, "'%hhd'\n", value);
	}

	void print_contract_violation(unsigned char const value) noexcept
	{
		(void)fprintf(stderr, "'%hhu'\n", value);
	}

	void print_contract_violation(short const value) noexcept
	{
		(void)fprintf(stderr, "'%hd'\n", value);
	}

	void print_contract_violation(unsigned short const value) noexcept
	{
		(void)fprintf(stderr, "'%hu'\n", value);
	}

	void print_contract_violation(int const value) noexcept
	{
		(void)fprintf(stderr, "'%d'\n", value);
	}

	void print_contract_violation(unsigned int const value) noexcept
	{
		(void)fprintf(stderr, "'%u'\n", value);
	}

	void print_contract_violation(long const value) noexcept
	{
		(void)fprintf(stderr, "'%ld'\n", value);
	}

	void print_contract_violation(unsigned long const value) noexcept
	{
		(void)fprintf(stderr, "'%lu'\n", value);
	}

	void print_contract_violation(long long const value) noexcept
	{
		(void)fprintf(stderr, "'%lld'\n", value);
	}

	void print_contract_violation(unsigned long long const value) noexcept
	{
		(void)fprintf(stderr, "'%llu'\n", value);
	}

	void print_contract_violation(float const value) noexcept
	{
		(void)fprintf(stderr, "'%f'\n", static_cast<double>(value));
	}

	void print_contract_violation(double const value) noexcept
	{
		(void)fprintf(stderr, "'%f'\n", value);
	}

	void print_contract_violation(long double const value) noexcept
	{
		(void)fprintf(stderr, "'%Lf'\n", value);
	}

	void print_contract_violation(bool const value) noexcept
	{
		(void)fprintf(stderr, "'%s'\n", value ? "true" : "false");
	}

	void print_contract_violation(char const value) noexcept
	{
		(void)fprintf(stderr, "'%c'\n", value);
	}

	void print_contract_violation(char const* const value) noexcept
	{
		(void)fprintf(stderr, "'%s'\n", value);
	}

	void print_contract_violation(wchar_t const value) noexcept
	{
		(void)fprintf(stderr, "'%lc'\n", value);
	}

	void print_contract_violation(wchar_t const* const value) noexcept
	{
		(void)fprintf(stderr, "'%ls'\n", value);
	}

	void print_contract_violation(char8_t const value) noexcept
	{
		(void)fprintf(stderr, "'%c'\n", static_cast<char>(value));
	}

	void print_contract_violation(char16_t const value) noexcept
	{
		(void)fprintf(stderr, "'%lc'\n", value);
	}

	void print_contract_violation(char32_t const value) noexcept
	{
		(void)fprintf(stderr, "'%c'\n", value);
	}

	void print_contract_violation(void const* const value) noexcept
	{
		(void)fprintf(stderr, "'%p'\n", value);
	}

	void print_contract_violation(std::string_view const value) noexcept
	{
		(void)print_contract_violation(value.data());
	}

	void print_contract_violation() noexcept
	{
		(void)fprintf(stderr, "<non-printable-type>");
	}

	void print_contract_violation_hex(unsigned char const value) noexcept
	{
		(void)fprintf(stderr, "'%#x'", value);
	}

	void print_contract_violation_header(
	  char const* const contract_type,
	  char const* const function_name,
	  char const* const expected) noexcept
	{
		(void)fprintf(
		  stderr,
		  "---------------------------------------- Contract report ----------------------------------------\n"
		  "%s of '%s' is that '%s', but we provided ",
		  contract_type,
		  function_name,
		  expected);
	}
} // namespace cjdb::constexpr_contracts_detail
