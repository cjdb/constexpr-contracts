// Copyright (c) Christopher Di Bella.
// Copyright (c) Casey Carter.
// Copyright (c) Eric Niebler.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#ifndef CJDB_CONTRACTS_HPP
#define CJDB_CONTRACTS_HPP

#include <string_view>
#include <type_traits>
namespace cjdb::constexpr_contracts_detail {
	void print_stacktrace() noexcept;

	void print_contract_violation(signed char value) noexcept;
	void print_contract_violation(unsigned char value) noexcept;
	void print_contract_violation(short value) noexcept;
	void print_contract_violation(unsigned short value) noexcept;
	void print_contract_violation(int value) noexcept;
	void print_contract_violation(unsigned int value) noexcept;
	void print_contract_violation(long value) noexcept;
	void print_contract_violation(unsigned long value) noexcept;
	void print_contract_violation(long long value) noexcept;
	void print_contract_violation(unsigned long long value) noexcept;
	void print_contract_violation(float value) noexcept;
	void print_contract_violation(double value) noexcept;
	void print_contract_violation(long double value) noexcept;
	void print_contract_violation(bool value) noexcept;
	void print_contract_violation(char value) noexcept;
	void print_contract_violation(char const* value) noexcept;
	void print_contract_violation(wchar_t value) noexcept;
	void print_contract_violation(wchar_t const* value) noexcept;
	void print_contract_violation(char8_t value) noexcept;
	void print_contract_violation(char16_t value) noexcept;
	void print_contract_violation(char32_t value) noexcept;
	void print_contract_violation(void const* value) noexcept;
	void print_contract_violation(std::string_view value) noexcept;
	void print_contract_violation() noexcept;
	void print_contract_violation_hex(unsigned char value) noexcept;
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

		constexpr void operator<=>(auto const& u) noexcept
		{
			dismissed_ = true;
			if (not (value_ <=> u)) [[unlikely]] {
				handle_violation(u);
			}
		}
	private:
		char const* function_;
		char const* expected_;
		T value_;
		bool dismissed_ = false;

		template<class U>
		[[noreturn]] constexpr void handle_violation(U const&) const noexcept
		{
			if (not std::is_constant_evaluated()) {
				report_failure(function_, expected_, value_);
			}

#if _MSC_VER
			__assume(0);
#elif defined(__OPTIMIZE__)
			__builtin_unreachable();
#else
			__builtin_trap();
#endif
		}

		constexpr auto eval() noexcept
		{
			if constexpr (requires { not value_; }) {
				return not static_cast<bool>(value_);
			}
			else {
				return true;
			}
		}

		static void report_failure(
		  [[maybe_unused]] char const* const function_name,
		  [[maybe_unused]] char const* const expected,
		  [[maybe_unused]] T const& actual) noexcept
		{
#if not defined(NDEBUG) // or defined(CJDB_CONTRACTS_ALWAYS_REPORT)
			print_contract_violation_header(contract_type(), function_name, expected);
			print_contract_violation(actual);
			print_stacktrace();
#endif
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

#define CJDB_ASSERT(...)                                                                                      \
	(void)(cjdb::constexpr_contracts_detail::contract<cjdb::constexpr_contracts_detail::contract_kind::assume>( \
	         __PRETTY_FUNCTION__,                                                                               \
	         #__VA_ARGS__)                                                                                      \
	         ->*__VA_ARGS__)
#define CJDB_EXPECTS(...)                                                                                      \
	(void)(cjdb::constexpr_contracts_detail::contract<cjdb::constexpr_contracts_detail::contract_kind::expects>( \
	         __PRETTY_FUNCTION__,                                                                                \
	         #__VA_ARGS__)                                                                                       \
	         ->*__VA_ARGS__)
#define CJDB_ENSURES(...)                                                                                      \
	(void)(cjdb::constexpr_contracts_detail::contract<cjdb::constexpr_contracts_detail::contract_kind::ensures>( \
	         __PRETTY_FUNCTION__,                                                                                \
	         #__VA_ARGS__)                                                                                       \
	         ->*__VA_ARGS__)

#endif // CJDB_CONTRACTS_HPP
