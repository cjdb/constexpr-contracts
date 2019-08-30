# Contracts Consolation

C++20 was looking good to have contracts until the C++ Standards Committee meeting in Cologne, Germany
of 2019. There, some strong turbulence happened, and the committee ultimately felt that significantly
more work was necessary before contracts are ready for Standard C++.

As one of the authors requesting contracts be removed from the C++20 Working Paper, I'd like to offer
a consolation library, which emulates how I'd like contracts to behave.

## Prerequisites

1. CMake 3.14 or later
2. Conan 1.18 or later (if using Conan)
4. A standard library that supports `std::is_constant_evaluated` (e.g. libstdc++11-9 or libc++ 9)

## Installation

### Without Conan

You can clone this repository and place the directory `cjdb` in your own `include` directory.

### With Conan

1. Add `artifactory` to your list of remotes.
2. Add `package-name` to your list of required packages.

## Usage

This library provides six macros:

<table>
	<thead>
		<th>Contract type</th>
		<th>Macro</th>
	</thead>
	<tbody>
		<tr>
			<td>Pre-condition</td>
			<td><code>CJDB_EXPECTS</code></td>
		</tr>
		<tr>
			<td>Assertion</td>
			<td><code>CJDB_ASSERT</code></td>
		</tr>
		<tr>
			<td>Post-condition</td>
			<td><code>CJDB_ENSURES</code></td>
		</tr>
	</tbody>
</table>

When `DNDEBUG` is a defined macro, the check will still happen, but the diagnostic won't be output. If
optimisations are enabled and `DNDEBUG` is defined, then the code generation has the potential to be
significantly different, since the optimiser will have stronger confidence about what code can be
reached with particular values. [See for yourself][__builtin_unreachable].

Also unlike `assert`, or the Guideline Support Library's `Expects` and `Ensures` pseudo-contracts, the
Contracts Consolation library works at both compile-time and run-time, meaning you can now express
assertions that might happen at compile-time or at run-time in your `constexpr` functions.

### Assertions

You should use assertions to indicate when a programming error has occurred in the middle of your
code. For example, the following program performs division operation, and checks that `f` never
returns `0`. If `f` returns zero, then the programmer has communicated that there's a logic error
somewhere.

```cpp
// file: example1.cpp
int main()
{
	std::integral auto x = f();
	CJDB_ASSERT(x != 0);
	std::cout << (5 / x) << '\n';
}
```

The above program will behave as if the assertion is not present in the event `x != 0`. If, however,
`x == 0`, the program will print the following message in debug-mode and then crash.
```
./example.cpp:5: assertion `x != 0` failed in `int main()`
```

In this case, nothing is different with `-O3 -DNDEBUG`, but [take a look][unreachable-2] at what
happens when the compiler can see what we're expecting.

### Pre-conditions

You should use a pre-condition to indicate that the expected input for a function is not met. Although
it can't be compiler-enforced, you should only use a pre-condition to check that a parameter doesn't
meet its expected input values; everything else should be checked using an assertion. This will help
your users determine when they have provided bad parameters, and when their state is the cause of a
logic error.

```cpp
// example2.cpp
class person {
public:
	constexpr explicit person(std::string_view const first_name, std::string_view const surname, int const age)
	: first_name_{first_name}
	, surname_{surname}
	, age_{(CJDB_EXPECTS(age >= 0), age)} // parens are necessary!
	{}
private:
	std::string first_name_;
	std::string surname_;
	int age_;
};

auto const ada_lovelace = person{
	"Ada",
	"Lovelace",
	-1
};
```

In our second example, we're establishing that a person's age must be `0` or greater. We do this
before we initialise `age_`, so that we know that the expectation is met before we ever get a chance
to use it. The run-time diagnostic we get is:

```
./example2.cpp:9: pre-condition `age >= 0` failed in `person::person(std::string_view, std::string_view, int)`
```

### Post-conditions

Finally, post-conditions allow us to provide a guarantee that our invariants hold upon exiting a
function. These are a bit different in usage: instead of providing just an expression, the intended
usage is to place your post-condition in a return-statement. As such, there should only be one
post-condition per return-statement in a function.

```cpp
int f()
{
	auto result = g();
	return CJDB_ENSURES(result >= 0), result;
}
```

This usage lets us chain post-conditions before our result is returned. As with pre-conditions and
assertions, the diagnostic indicates what kind of contract was violated.

## Contracts Consolation in constant expressions

Notice that `person::person` is `constexpr`. Contracts Consolation has a huge advantage over both
`assert` and `static_assert`, because it _can_ be evaluated at compile-time or at run-time, depending
on the context. If we wanted to constant-initialise `ada_lovelace`, then we'd get a compile-time
diagnostic instead. They're fairly difficult to read, so I've put one below to guide you.

### Clang

```
constexpr auto ada_lovelace = person{
               ^              ~~~~~~~
../include/cjdb/contracts.hpp:57:4: note: subexpression not valid in a constant expression
                        __builtin_unreachable();
                        ^
../test/person.cpp:8:10: note: in call to 'contract_impl(false, {&"../test/person.cpp:8: pre-condition `age >= 0` failed in `%s`\n"[0], 62}, {&__PRETTY_FUNCTION__[0], 73})'
        , age_{(CJDB_EXPECTS(age >= 0), age)}
                ^
../include/cjdb/contracts.hpp:29:27: note: expanded from macro 'CJDB_EXPECTS'
#define CJDB_EXPECTS(...) CJDB_CONTRACT_IMPL("pre-condition", __VA_ARGS__)
                          ^
../include/cjdb/contracts.hpp:66:4: note: expanded from macro 'CJDB_CONTRACT_IMPL'
   ::cjdb::contracts_detail::contract_impl(static_cast<bool>(__VA_ARGS__),    \
   ^
../test/person.cpp:16:31: note: in call to 'person({&"Ada"[0], 3}, {&"Lovelace"[0], 8}, -1)'
constexpr auto ada_lovelace = person{
                              ^
```

That's a lot of error to say that `age < 0`! We can fortunately ignore 90% of the above. The only
useful information in the entire diagnostic for our purposes are the first eight lines.

```
constexpr auto ada_lovelace = person{
               ^              ~~~~~~~
../include/cjdb/contracts.hpp:57:4: note: subexpression not valid in a constant expression
                        __builtin_unreachable();
                        ^
../test/person.cpp:8:10: note: in call to 'contract_impl(false, {&"../test/person.cpp:8: pre-condition `age >= 0` failed in `%s`\n"[0], 62}, {&__PRETTY_FUNCTION__[0], 73})'
        , age_{(CJDB_EXPECTS(age >= 0), age)}
                ^
```

Here, we see that `constexpr auto ada_lovelace = person{` is the offending call-site, and that
`CJDB_EXPECTS(age >= 0)` is the contract that we broke. Literally everything else is helper
information that the compiler needs to give us, but we don't need.

### GCC

```
In file included from ../test/person.cpp:1:
../test/person.cpp:20:1:   in ‘constexpr’ expansion of ‘person(std::basic_string_view<char>(((const char*)"Ada")), std::basic_string_view<char>(((const char*)"Lovelace")), -1)’
../test/person.cpp:8:10:   in ‘constexpr’ expansion of ‘cjdb::contracts_detail::contract_impl(((int)(((int)age) >= 0)), std::basic_string_view<char>(((const char*)"../test/person.cpp:8: pre-condition `age >= 0` failed in `%s`\012")), std::basic_string_view<char>(((const char*)(& __PRETTY_FUNCTION__))))’
../include/cjdb/contracts.hpp:57:25: error: ‘__builtin_unreachable()’ is not a constant expression
   57 |    __builtin_unreachable();
      |    ~~~~~~~~~~~~~~~~~~~~~^~
```

Sadly, GCC's diagnostic is much more technical, and harder to read. You'll need to look for
`cjdb::contracts_detail::contract_impl(((int)(((int)age) >= 0))`.
`cjdb::contracts_detail::contract_impl` is an implementation detail, so only look for it in
diagnostics, as it is subject to change. As such, I'm working on a way to provide a _much_ better
diagnostic.

## Pitfalls

Because we're peering into the definition here, the code is somewhat brittle: any user-facing
declaration needs to match the definition right down to parameter names; otherwise the contract won't
make much sense to the user.

[__builtin_unreachable]: https://gcc.godbolt.org/z/5iRsJW
[unreachable-2]: https://gcc.godbolt.org/z/xOBmL6
