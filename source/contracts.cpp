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
		  "\n------------------------------------------ Stack trace ------------------------------------------\n");

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
