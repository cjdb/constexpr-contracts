// Copyright (c) Google LLC.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#ifndef CJDB_DETAIL_PRINT_CONTRACT_VIOLATION_HPP
#define CJDB_DETAIL_PRINT_CONTRACT_VIOLATION_HPP

#include <compare>
#include <complex>
#include <cstddef>
#include <cstdio>
#include <iterator>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace cjdb::constexpr_contracts_detail {
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
	void print_contract_violation(wchar_t value) noexcept;
	void print_contract_violation(char8_t value) noexcept;
	void print_contract_violation(char16_t value) noexcept;
	void print_contract_violation(char32_t value) noexcept;
	void print_contract_violation(std::byte value) noexcept;
	void print_contract_violation(std::nullptr_t value) noexcept;
	void print_contract_violation(std::nullopt_t value) noexcept;
	void print_contract_violation(std::partial_ordering value) noexcept;
	void print_contract_violation(std::weak_ordering value) noexcept;
	void print_contract_violation(std::strong_ordering value) noexcept;

	template<class T>
	void print_contract_violation(T* value) noexcept;
	template<class Traits>
	void print_contract_violation(std::basic_string_view<char, Traits> value) noexcept;
	template<class Traits>
	void print_contract_violation(std::basic_string_view<wchar_t, Traits> value) noexcept;
	template<class CharT, class Traits>
	void print_contract_violation(std::basic_string_view<CharT, Traits> value) noexcept;
	template<class CharT, class Traits, class Alloc>
	void print_contract_violation(std::basic_string<CharT, Traits, Alloc> const& value) noexcept;
	template<class T>
	void print_contract_violation(std::complex<T> value) noexcept;
	template<class T, class U>
	void print_contract_violation(std::pair<T, U> const& value) noexcept;
	template<class... Args>
	void print_contract_violation(std::tuple<Args...> const& value) noexcept;
	template<class T>
	void print_contract_violation(std::optional<T> const& value) noexcept;
	template<class T, class Deleter>
	void print_contract_violation(std::unique_ptr<T, Deleter> const& value) noexcept;
	template<class T>
	void print_contract_violation(std::shared_ptr<T> const& value) noexcept;

	template<class T>
	concept __enum = std::is_enum_v<T>;

	template<__enum T>
	void print_contract_violation(T const& value) noexcept;
	template<std::ranges::input_range R>
	void print_contract_violation(R& r) noexcept;
	template<std::contiguous_iterator I>
	void print_contract_violation(I const& value) noexcept;

	void print_contract_violation(auto const&) noexcept
	{
		(void)fputs("<non-printable-type>", stderr);
	}

	template<class T>
	void print_contract_violation(T* const value) noexcept
	{
		if (value == nullptr) {
			return print_contract_violation(nullptr);
		}

		(void)fprintf(stderr, "%p", static_cast<void const*>(value));
	}

	template<class Traits>
	void print_contract_violation(std::basic_string_view<char, Traits> const value) noexcept
	{
		(void)fprintf(stderr, "\"%s\"", value.data());
	}

	template<class Traits>
	void print_contract_violation(std::basic_string_view<wchar_t, Traits> const value) noexcept
	{
		(void)fprintf(stderr, "\"%ls\"", value.data());
	}

	template<class CharT, class Traits>
	void print_contract_violation(std::basic_string_view<CharT, Traits> const value) noexcept
	{
		(void)fputs("\"", stderr);
		for (auto const i : value) {
			(void)fprintf(stderr, "%c", static_cast<char>(i));
		}
		(void)fputs("\"", stderr);
	}

	template<class CharT, class Traits, class Alloc>
	void print_contract_violation(std::basic_string<CharT, Traits, Alloc>& value) noexcept
	{
		print_contract_violation(std::basic_string_view<CharT, Traits>(value));
	}

	template<class CharT, class Traits, class Alloc>
	void print_contract_violation(std::basic_string<CharT, Traits, Alloc> const& value) noexcept
	{
		print_contract_violation(std::basic_string_view<CharT, Traits>(value));
	}

	template<class T>
	void print_contract_violation(std::complex<T> const value) noexcept
	{
		print_contract_violation(value.real());
		(void)fputs(" + ", stderr);
		print_contract_violation(value.imag());
		(void)fputs("i", stderr);
	}

	template<class T, class U>
	void print_contract_violation(std::pair<T, U> const& value) noexcept
	{
		(void)fputs("(", stderr);
		print_contract_violation(value.first);
		(void)fputs(", ", stderr);
		print_contract_violation(value.second);
		(void)fputs(")", stderr);
	}

	template<class... Args, std::size_t... N>
	void print_tuple(std::tuple<Args...> const& value, std::index_sequence<N...>) noexcept
	{
		print_contract_violation(std::get<0>(value));
		(((void)fputs(", ", stderr), print_contract_violation(std::get<N + 1>(value))), ...);
	}

	template<class... Args>
	void print_contract_violation(std::tuple<Args...> const& value) noexcept
	{
		(void)fputs("(", stderr);
		if constexpr (sizeof...(Args) > 0) {
			print_tuple(value, std::make_index_sequence<sizeof...(Args) - 1>{});
		}
		(void)fputs(")", stderr);
	}

	template<class T>
	void print_contract_violation(std::optional<T> const& value) noexcept
	{
		if (value) {
			return print_contract_violation(*value);
		}

		return print_contract_violation(std::nullopt);
	}

	template<class T, class Deleter>
	void print_contract_violation(std::unique_ptr<T, Deleter> const& value) noexcept
	{
		print_contract_violation(value.get());
	}

	template<class T>
	void print_contract_violation(std::shared_ptr<T> const& value) noexcept
	{
		print_contract_violation(value.get());
	}

	template<__enum T>
	void print_contract_violation(T const& value) noexcept
	{
		print_contract_violation(static_cast<std::underlying_type_t<T>>(value));
	}

	template<std::forward_iterator I, std::sentinel_for<I> S>
	void print_contract_violation(I first, S last)
	{
		if (first == last) {
			return;
		}

		print_contract_violation(*first);
		while (++first != last) {
			(void)fputs(", ", stderr);
			print_contract_violation(*first);
		}
	}

	template<std::ranges::forward_range R>
	void print_contract_violation(R& r) noexcept
	{
		(void)fputs("[", stderr);
		print_contract_violation(std::ranges::begin(r), std::ranges::end(r));
		(void)fputs("]", stderr);
	}

	template<std::contiguous_iterator I>
	void print_contract_violation(I const& value) noexcept
	{
		print_contract_violation(std::to_address(value));
	}
} // namespace cjdb::constexpr_contracts_detail

#endif // CJDB_DETAIL_PRINT_CONTRACT_VIOLATION_HPP
