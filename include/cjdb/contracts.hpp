// Copyright (c) Google LLC.
// Copyright (c) Christopher Di Bella.
// Copyright (c) Casey Carter.
// Copyright (c) Eric Niebler.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#ifndef CJDB_CONTRACTS_HPP
#define CJDB_CONTRACTS_HPP

#include "detail/print_contract_violation.hpp"
#include <type_traits>
#include <utility>

namespace cjdb::constexpr_contracts_detail {
	void print_stacktrace() noexcept;

	void
	print_contract_violation_header(char const* contract_type, char const* function_name, char const* expected) noexcept;

	enum class contract_kind { expects, ensures, assume };

	template<class T, contract_kind Kind>
	class contract_impl {
	public:
		constexpr contract_impl(char const* const function, char const* const expected, T&& actual) noexcept
		: function_(function)
		, expected_(expected)
		, value_(static_cast<T&&>(actual))
		{}

		constexpr ~contract_impl() noexcept
		{
			if (not dismissed_ and eval()) {
				constexpr auto meaningless_number = 42;
				handle_violation(meaningless_number);
			}
		}

		contract_impl(contract_impl const&) = delete;
		contract_impl& operator=(contract_impl const&) = delete;

		constexpr void operator==(auto const& u) noexcept
		{
			dismissed_ = true;
			if (not (value_ == u)) [[unlikely]] {
				handle_violation(u);
			}
		}

		constexpr void operator!=(auto const& u) noexcept
		{
			dismissed_ = true;
			if (not (value_ != u)) [[unlikely]] {
				handle_violation(u);
			}
		}

		constexpr void operator<(auto const& u) noexcept
		{
			dismissed_ = true;
			if (not (value_ < u)) [[unlikely]] {
				handle_violation(u);
			}
		}

		constexpr void operator>(auto const& u) noexcept
		{
			dismissed_ = true;
			if (not (value_ > u)) [[unlikely]] {
				handle_violation(u);
			}
		}

		constexpr void operator<=(auto const& u) noexcept
		{
			dismissed_ = true;
			if (not (value_ <= u)) [[unlikely]] {
				handle_violation(u);
			}
		}

		constexpr void operator>=(auto const& u) noexcept
		{
			dismissed_ = true;
			if (not (value_ >= u)) [[unlikely]] {
				handle_violation(u);
			}
		}
	private:
		char const* function_;
		char const* expected_;
		T value_;
		bool dismissed_ = false;

		template<class U>
		[[noreturn]] constexpr void handle_violation(U const&) noexcept
		{
			if (not std::is_constant_evaluated()) {
				report_failure(function_, expected_, value_);
			}

#if _MSC_VER
			__assume(0);
#else
			__builtin_abort();
#endif
		}

		constexpr auto eval() noexcept
		{
			if constexpr (std::is_convertible_v<T, bool>) {
				return not static_cast<bool>(value_);
			}
			else {
				return true;
			}
		}

		static void report_failure(char const* const function_name, char const* const expected, T& actual) noexcept
		{
			print_contract_violation_header(contract_type(), function_name, expected);
			(void)fputs("'", stderr);
			print_contract_violation(actual);
			(void)fputs("'", stderr);
			print_stacktrace();
		}

		static constexpr char const* contract_type() noexcept
		{
			return Kind == contract_kind::expects ? "A precondition"
			     : Kind == contract_kind::ensures ? "A postcondition"
			     : Kind == contract_kind::assume
			       ? "An assumption"
			       : "<internal error: new kind of contract not handled>";
		}
	};

	template<contract_kind Kind>
	class contract {
	public:
		constexpr contract(char const* const function, char const* const expected) noexcept
		: function_(function)
		, expected_(expected)
		{}

		template<class T>
		constexpr contract_impl<T, Kind> operator->*(T&& t) noexcept
		{
			return {function_, expected_, static_cast<T&&>(t)};
		}
	private:
		char const* function_;
		char const* expected_;
	};
} // namespace cjdb::constexpr_contracts_detail

#if defined(CJDB_CONTRACTS_DISABLED)
#	define CJDB_ASSERT(...)  (void)(false)
#	define CJDB_EXPECTS(...) (void)(false)
#	define CJDB_ENSURES(...) (void)(false)
#else
#	define CJDB_ASSERT(...)                                                                                      \
		(void)(cjdb::constexpr_contracts_detail::contract<cjdb::constexpr_contracts_detail::contract_kind::assume>( \
		         __PRETTY_FUNCTION__,                                                                               \
		         #__VA_ARGS__)                                                                                      \
		         ->*__VA_ARGS__)
#	define CJDB_EXPECTS(...)                                                                                      \
		(void)(cjdb::constexpr_contracts_detail::contract<cjdb::constexpr_contracts_detail::contract_kind::expects>( \
		         __PRETTY_FUNCTION__,                                                                                \
		         #__VA_ARGS__)                                                                                       \
		         ->*__VA_ARGS__)
#	define CJDB_ENSURES(...)                                                                                      \
		(void)(cjdb::constexpr_contracts_detail::contract<cjdb::constexpr_contracts_detail::contract_kind::ensures>( \
		         __PRETTY_FUNCTION__,                                                                                \
		         #__VA_ARGS__)                                                                                       \
		         ->*__VA_ARGS__)
#endif

#endif // CJDB_CONTRACTS_HPP
