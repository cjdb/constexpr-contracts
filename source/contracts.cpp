// Copyright (c) Google LLC
// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0
//

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cxxabi.h>
#include <execinfo.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <ranges>
#include <span>
#include <string>
#include <string_view>

namespace cjdb::constexpr_contracts_detail {
	namespace {
		[[nodiscard]] auto find_last(auto& r, auto const& value)
		{
			auto last = r.end();
			for (auto i = r.begin(); i != r.end(); ++i) {
				if (i->contains(value)) {
					last = i;
				}
			}

			return last;
		}
	} // namespace

	void print_stacktrace() noexcept
	{
		(void)fprintf(
		  stderr,
		  "\n\n------------------------------------------ Stack trace ------------------------------------------\n");

		auto raw_message = std::string();
		auto os = llvm::raw_string_ostream(raw_message);
		llvm::sys::PrintStackTrace(os);
		auto stacktrace = llvm::SmallVector<llvm::StringRef, 0>();
		llvm::StringRef(raw_message).split(stacktrace, '\n');

		auto frame = 0;
		for (auto i = find_last(stacktrace, "cjdb::constexpr_contracts_detail::contract_impl") + 1; i != stacktrace.end(); ++i)
		{
			if (i->empty()) {
				continue;
			}

			auto const location = i->find("0x");
			auto const current_trace = std::string(i->begin() + location, i->begin() + i->size());
			(void)std::fprintf(stderr, "#%i %s\n", frame++, current_trace.data());
		}
	}

	void print_contract_violation(signed char const value) noexcept
	{
		(void)fprintf(stderr, "%hhd", value);
	}

	void print_contract_violation(unsigned char const value) noexcept
	{
		(void)fprintf(stderr, "%hhu", value);
	}

	void print_contract_violation(short const value) noexcept
	{
		(void)fprintf(stderr, "%hd", value);
	}

	void print_contract_violation(unsigned short const value) noexcept
	{
		(void)fprintf(stderr, "%hu", value);
	}

	void print_contract_violation(int const value) noexcept
	{
		(void)fprintf(stderr, "%d", value);
	}

	void print_contract_violation(unsigned int const value) noexcept
	{
		(void)fprintf(stderr, "%u", value);
	}

	void print_contract_violation(long const value) noexcept
	{
		(void)fprintf(stderr, "%ld", value);
	}

	void print_contract_violation(unsigned long const value) noexcept
	{
		(void)fprintf(stderr, "%lu", value);
	}

	void print_contract_violation(long long const value) noexcept
	{
		(void)fprintf(stderr, "%lld", value);
	}

	void print_contract_violation(unsigned long long const value) noexcept
	{
		(void)fprintf(stderr, "%llu", value);
	}

	void print_contract_violation(float const value) noexcept
	{
		(void)fprintf(stderr, "%a", static_cast<double>(value));
	}

	void print_contract_violation(double const value) noexcept
	{
		(void)fprintf(stderr, "%a", value);
	}

	void print_contract_violation(long double const value) noexcept
	{
		(void)fprintf(stderr, "%La", value);
	}

	void print_contract_violation(bool const value) noexcept
	{
		(void)fprintf(stderr, "%s", value ? "true" : "false");
	}

	void print_contract_violation(char const value) noexcept
	{
		(void)fprintf(stderr, "%c", value);
	}

	void print_contract_violation(wchar_t const value) noexcept
	{
		(void)fprintf(stderr, "%lc", value);
	}

	void print_contract_violation(char8_t const value) noexcept
	{
		(void)fprintf(stderr, "%c", static_cast<char>(value));
	}

	void print_contract_violation(char16_t const value) noexcept
	{
		(void)fprintf(stderr, "%lc", value);
	}

	void print_contract_violation(char32_t const value) noexcept
	{
		(void)fprintf(stderr, "%c", value);
	}

	void print_contract_violation(std::byte const value) noexcept
	{
		(void)fprintf(stderr, "0x%02x", static_cast<unsigned char>(value));
	}

	void print_contract_violation(std::nullptr_t) noexcept
	{
		(void)fputs("nullptr", stderr);
	}
	void print_contract_violation(std::nullopt_t) noexcept
	{
		(void)fputs("std::nullopt", stderr);
	}

	void print_contract_violation(std::partial_ordering const value) noexcept
	{
		if (value == std::partial_ordering::less) {
			return (void)fputs("std::partial_ordering::less", stderr);
		}

		if (value == std::partial_ordering::equivalent) {
			return (void)fputs("std::partial_ordering::equivalent", stderr);
		}

		if (value == std::partial_ordering::greater) {
			return (void)fputs("std::partial_ordering::greater", stderr);
		}

		return (void)fputs("std::partial_ordering::unordered", stderr);
	}

	void print_contract_violation(std::weak_ordering const value) noexcept
	{
		if (value == std::weak_ordering::less) {
			return (void)fputs("std::weak_ordering::less", stderr);
		}

		if (value == std::weak_ordering::equivalent) {
			return (void)fputs("std::weak_ordering::equivalent", stderr);
		}

		return (void)fputs("std::weak_ordering::greater", stderr);
	}

	void print_contract_violation(std::strong_ordering const value) noexcept
	{
		if (value == std::strong_ordering::less) {
			return (void)fputs("std::strong_ordering::less", stderr);
		}

		if (value == std::strong_ordering::equal) {
			return (void)fputs("std::strong_ordering::equal", stderr);
		}

		return (void)fputs("std::strong_ordering::greater", stderr);
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
