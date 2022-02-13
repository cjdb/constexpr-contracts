// Copyright (c) Google LLC.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//

// This file goes way outside the realm of "don't test implementation details", but the alternative
// is to assert for each and every type, which will be fairly cumbersome. This allows us to quickly
// check everything in one fell swoop.

#include "cjdb/contracts.hpp"
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <optional>
#include <string>
#include <vector>

template<class T>
void check_print_contract_violation(T t) noexcept
{
	using cjdb::constexpr_contracts_detail::print_contract_violation;
	print_contract_violation(t);
	(void)fputs("\n", stderr);
}

template<std::integral I>
void check_print_contract_violation() noexcept
{
	check_print_contract_violation(static_cast<I>(-3));
	check_print_contract_violation(static_cast<I>(-2));
	check_print_contract_violation(static_cast<I>(-1));
	check_print_contract_violation(static_cast<I>(0));
	check_print_contract_violation(static_cast<I>(1));
	check_print_contract_violation(static_cast<I>(2));
	check_print_contract_violation(static_cast<I>(3));
}

template<std::floating_point T>
void check_print_contract_violation() noexcept
{
	check_print_contract_violation(static_cast<T>(-3.521));
	check_print_contract_violation(static_cast<T>(-2.0412e10));
	check_print_contract_violation(static_cast<T>(-0.5421));
	check_print_contract_violation(static_cast<T>(-0.0));
	check_print_contract_violation(static_cast<T>(0.0));
	check_print_contract_violation(static_cast<T>(0.1));
	check_print_contract_violation(static_cast<T>(2.0e-10));
	check_print_contract_violation(static_cast<T>(3.0));
}

int main()
{
	check_print_contract_violation<signed char>();
	check_print_contract_violation<unsigned char>();
	check_print_contract_violation<short>();
	check_print_contract_violation<unsigned short>();
	check_print_contract_violation<int>();
	check_print_contract_violation<unsigned int>();
	check_print_contract_violation<long>();
	check_print_contract_violation<unsigned long>();
	check_print_contract_violation<long long>();
	check_print_contract_violation<unsigned long long>();

	check_print_contract_violation<float>();
	check_print_contract_violation<double>();
	check_print_contract_violation<long double>();

	check_print_contract_violation('0');
	check_print_contract_violation('A');
	check_print_contract_violation('b');
	check_print_contract_violation('~');
	check_print_contract_violation(L'0');
	check_print_contract_violation(L'A');
	check_print_contract_violation(L'b');
	check_print_contract_violation(L'~');
	check_print_contract_violation(u8'0');
	check_print_contract_violation(u8'A');
	check_print_contract_violation(u8'b');
	check_print_contract_violation(u8'~');
	check_print_contract_violation(u'0');
	check_print_contract_violation(u'A');
	check_print_contract_violation(u'b');
	check_print_contract_violation(u'~');
	check_print_contract_violation(U'0');
	check_print_contract_violation(U'A');
	check_print_contract_violation(U'b');
	check_print_contract_violation(U'~');

	auto i = 0;
	check_print_contract_violation(&i);

	auto const c = 'X';
	check_print_contract_violation(&c);

	auto const wc = L'X';
	check_print_contract_violation(&wc);

	check_print_contract_violation("Hello");
	check_print_contract_violation(L"Hello");
	check_print_contract_violation(U"Hello");
	check_print_contract_violation(u"Hello");
	check_print_contract_violation(u8"Hello");

	check_print_contract_violation(nullptr);

	int const* p = nullptr;
	check_print_contract_violation(p);

	check_print_contract_violation(std::byte{0});
	check_print_contract_violation(std::byte{1});
	check_print_contract_violation(std::byte{2});
	check_print_contract_violation(std::byte{3});
	check_print_contract_violation(std::partial_ordering::less);
	check_print_contract_violation(std::partial_ordering::equivalent);
	check_print_contract_violation(std::partial_ordering::greater);
	check_print_contract_violation(std::partial_ordering::unordered);
	check_print_contract_violation(std::weak_ordering::less);
	check_print_contract_violation(std::weak_ordering::equivalent);
	check_print_contract_violation(std::weak_ordering::greater);
	check_print_contract_violation(std::strong_ordering::less);
	check_print_contract_violation(std::strong_ordering::equivalent);
	check_print_contract_violation(std::strong_ordering::equal);
	check_print_contract_violation(std::strong_ordering::greater);
	check_print_contract_violation(std::string_view("world"));
	check_print_contract_violation(std::string("12345"));
	check_print_contract_violation(std::wstring_view(L"world"));
	check_print_contract_violation(std::wstring(L"12345"));
	check_print_contract_violation(std::u8string_view(u8"world"));
	check_print_contract_violation(std::u8string(u8"12345"));
	check_print_contract_violation(std::u16string_view(u"world"));
	check_print_contract_violation(std::u16string(u"12345"));
	check_print_contract_violation(std::u32string_view(U"world"));
	check_print_contract_violation(std::u32string(U"12345"));

	check_print_contract_violation(std::nullopt);
	check_print_contract_violation(std::optional<int>(0));
	check_print_contract_violation(std::optional<int>(1));
	check_print_contract_violation(std::optional<int>(2));
	check_print_contract_violation(std::optional<int>());
	check_print_contract_violation(std::optional<std::string_view>("abcde"));
	check_print_contract_violation(std::optional<std::string_view>("ghijk"));
	check_print_contract_violation(std::optional<std::string_view>("lmnop"));
	check_print_contract_violation(std::optional<std::string_view>());

	check_print_contract_violation(std::array{1, 2, 3, 4, 5});
	check_print_contract_violation(std::array{'a', 'b', 'c', 'd', 'e'});
	check_print_contract_violation(std::deque{1, 2, 3, 4, 5});
	check_print_contract_violation(std::deque{'a', 'b', 'c', 'd', 'e'});
	check_print_contract_violation(std::forward_list{1, 2, 3, 4, 5});
	check_print_contract_violation(std::forward_list{'a', 'b', 'c', 'd', 'e'});
	check_print_contract_violation(std::list{1, 2, 3, 4, 5});
	check_print_contract_violation(std::list{'a', 'b', 'c', 'd', 'e'});
	check_print_contract_violation(std::vector{1, 2, 3, 4, 5});
	check_print_contract_violation(std::vector{'a', 'b', 'c', 'd', 'e'});
	check_print_contract_violation(std::vector{'a', 'b', 'c', 'd', 'e'}.begin());
	check_print_contract_violation(std::vector{'a', 'b', 'c', 'd', 'e'}.end());

	check_print_contract_violation(std::pair{'a', 'b'});
	check_print_contract_violation(std::pair{1, 'a'});

	check_print_contract_violation(std::complex<float>(1, 0));
	check_print_contract_violation(std::complex<float>(1, 10));
	check_print_contract_violation(std::complex<double>(1, 0));
	check_print_contract_violation(std::complex<double>(1, 10));
	check_print_contract_violation(std::complex<long double>(1, 0));
	check_print_contract_violation(std::complex<long double>(1, 10));

	check_print_contract_violation(std::tuple<>());
	check_print_contract_violation(std::tuple<int>(0));
	check_print_contract_violation(std::tuple<int, std::string>(0, "hello"));
	check_print_contract_violation(std::tuple<int, std::string, float>(0, "hello", 1.23));

	enum { a, b };
	check_print_contract_violation(a);
	check_print_contract_violation(b);

	enum class scoped { a = -1, b = 82 };
	check_print_contract_violation(scoped::a);
	check_print_contract_violation(scoped::b);

	enum class unsigned_scoped : unsigned { a = ~0U, b = 82U };
	check_print_contract_violation(unsigned_scoped::a);
	check_print_contract_violation(unsigned_scoped::b);

	check_print_contract_violation(std::make_unique<int>(10));
	check_print_contract_violation(std::make_unique<std::string>("hello"));
	check_print_contract_violation(std::unique_ptr<int>());
}
