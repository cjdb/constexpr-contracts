# constexpr-contracts

C++20 was looking good to have contracts until the C++ Standards Committee meeting in Cologne,
Germany of 2019. There, some strong turbulence happened, and the committee ultimately felt that
significantly more work was necessary before contracts are ready for Standard C++.

As one of the authors requesting contracts be removed from the C++20 Working Paper, I'd like to
offer a consolation library, which emulates how I'd like contracts to behave.

## Prerequisites

1. CMake 3.17 or later
2. A compiler that supports `consteval` in C++20-mode.
3. A standard library that supports `std::is_constant_evaluated` in C++20-mode.

## Installation

### Without vcpkg

FIXME

### With vcpkg

FIXME

#### Linking in CMake

To link in CMake, you'll need to use `find_package(constexpr-contracts REQUIRED)` to import the library.
To link against the library, use `target_link_libraries(target PRIVATE cjdb::constexpr-contracts)`.

## Usage

This library provides three macros:

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

As the name of this library implies, you can use all of these macros in constant expressions. Your
contracts are always checked at compile-time. If the contract's predicate isn't satisfied, then the
program won't compile. This is a huge advantage over using `<cassert>` or the GSL's `Expects` and
`Ensures` macros.

When optimisations are diabled and `NDEBUG` is not defined as a macro, the contract will check your
predicate at run-time. If the predicate fails, then a diagnostic will be emit, and the program will
crash.

When optimisations are enabled, and `NDEBUG` remains undefined, the program will emit a diagnostic
for failed predicates, but the program **continues to run**, under the **assumption that the
predicate was actually true**. Your program will be slightly larger than if you hadn't used the
contract, but this might be useful for identifiying cases where users report bugs (in a release
build) that aren't being caught by your assertions.

When optimisations are disabled, but `NDEBUG` is defined as a macro, the program will crash when
the predicate fails, but no diagnostic is emitted. Code generation also appears to be significantly
worse.

When both optimisations and `NDEBUG` are enabled, the optimiser is allowed to make assumptions that
could improve code generation and no diagnostics are emitted. Toy examples demonstrate better code
generation than when the contract isn't used. [See for yourself][__builtin_unreachable].

<table>
	<thead>
		<th style="text-align:center;">Optimisations enabled</th>
		<th style="text-align:center;"><code>DNDEBUG</code> enabled</th>
		<th style="text-align:center;">Predicate evaluation</th>
		<th style="text-align:center;">Notes</th>
	</thead>
	<tbody style="text-align:center;">
		<tr>
			<td colspan="4">Constant expression</td>
		</tr>
		<tr>
			<td>N/A</td>
			<td>N/A</td>
			<td><code>false</code></td>
			<td>Program does not compile</td>
		</tr>
		<tr>
			<td>N/A</td>
			<td>N/A</td>
			<td><code>true</code></td>
			<td>Program compiles</td>
		</tr>
		<tr>
			<td colspan="4">Run-time expression</td>
		</tr>
		<tr>
			<td>No</td>
			<td>No</td>
			<td><code>false</code></td>
			<td style="text-align: left;">
				A diagnostic is written to <code>stderr</code> at run-time and then the program crashes.
			</td>
		</tr>
		<tr>
			<td>No</td>
			<td>Yes</td>
			<td><code>false</code></td>
			<td style="text-align: left;">
				The generated code will probably be slightly different than if <code>NDEBUG</code>
				hadn't been defined.<br />
				<br />
				A diagnostic is written to <code>stderr</code> at run-time and then the program crashes.
			</td>
		</tr>
		<tr>
			<td>Yes</td>
			<td>No</td>
			<td><code>false</code></td>
			<td style="text-align: left;">
				Optimiser considers predicate as absolute truth, which leads to optimisations that won't
				make sense with the given incorrect input.<br />
				<br />
				A diagnostic is written to <code>stderr</code> at run-time. The program won't crash, but
				it'll probably give incorrect output.
			</td>
		</tr>
		<tr>
			<td>Yes</td>
			<td>Yes</td>
			<td><code>false</code></td>
			<td style="text-align: left;">
				Optimiser considers predicate as absolute truth, which leads to optimisations that may
				result in a faster run-time or a smaller binary.<br />
				<br />
				The program won't crash, but it'll probably give incorrect output.
			</td>
		</tr>
		<tr>
			<td>No</td>
			<td>No</td>
			<td><code>true</code></td>
			<td style="text-align: left;">
				The program will take a performance hit (subject to the evaluation of the predicate plus
				a check to see that the predicate is <code>true</code>), but will otherwise behave as if
				the predicate didn't exist.
			</td>
		</tr>
		<tr>
			<td>No</td>
			<td>Yes</td>
			<td><code>true</code></td>
			<td style="text-align: left;">
				The program will take a performance hit (subject to the evaluation of the predicate plus
				a check to see that the predicate is <code>true</code>), but will otherwise behave as if
				the predicate didn't exist.
			</td>
		</tr>
		<tr>
			<td>Yes</td>
			<td>No</td>
			<td><code>true</code></td>
			<td style="text-align: left;">
				The optimiser considers the predicate to mean the absolute truth, and may make
				assumptions about code following the contract*. This might lead to optimisations
				resulting in a faster run-time or smaller binary.<br />
				<br />
				Correct program output is expected, and code generation is <em>expected</em> to be
				better than without optimisations, but the program will not be as small as when built
				using <code>-DNDEBUG</code>.
			</td>
		</tr>
		<tr>
			<td>Yes</td>
			<td>Yes</td>
			<td><code>true</code></td>
			<td style="text-align: left;">
				The optimiser considers the predicate to mean the absolute truth, and might make
				aggressive assumptions about code following the contract*. This may lead to
				optimisations resulting in a faster run-time, a smaller binary, or both.<br />
				<br />
				Correct program output is expected. Performance is subject to the level of optimisation,
				but this configuration is expected to result in the best code generation.
			</td>
		</tr>
	</tbody>
</table>

*Rudimentary testing has identified that neither GCC nor Clang perform optimisations <em>before</em>
the contract.

### Assertions

You should use assertions to indicate when a programming error has occurred in the middle of your
code. For example, the following program performs division operation, and checks that `f` never
returns `0`. If `f` returns zero, then the programmer has communicated that there's a logic error
somewhere.

```cpp
// file: example1.cpp
#include <cjdb/contracts.hpp>
#include <concepts>

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
./example.cpp:8: assertion `x != 0` failed in `int main()`
```

In this case, nothing is different with `-O3 -DNDEBUG`, but [take a look][unreachable-2] at what
happens when the compiler can see what we're expecting.

### Pre-conditions

You should use a pre-condition to indicate that the expected input for a function is not met.
Although it can't be compiler-enforced, you should only use a pre-condition to check that a
parameter doesn't meet its expected input values; everything else should be checked using an
assertion. This will help your users determine when they have provided bad parameters, and when
their state is the cause of a logic error.

```cpp
// example2.cpp
#include <cjdb/contracts.hpp>
#include <string_view>

class person {
public:
	constexpr explicit person(std::string_view first_name, std::string_view surname, int age)
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
./example2.cpp:10: pre-condition `age >= 0` failed in `person::person(std::string_view, std::string_view, int)`
```

### Post-conditions

Finally, post-conditions allow us to provide a guarantee that our invariants hold upon exiting a
function. These are a bit different in usage: instead of providing just an expression, the intended
usage is to place your post-condition in a return-statement. As such, there should only be one
post-condition per return-statement in a function.

```cpp
#include <cjdb/contracts.hpp>

int f()
{
	auto result = g();
	return CJDB_ENSURES(result >= 0), result;
}
```

This usage lets us chain post-conditions before our result is returned. As with pre-conditions and
assertions, the diagnostic indicates what kind of contract was violated.

## constexpr-contracts in constant expressions

Notice that `person::person` is `constexpr`. constexpr-contracts has a huge advantage over both
`assert` and `static_assert`, because it _can_ be evaluated at compile-time or at run-time,
depending on the context. If we wanted to constant-initialise `ada_lovelace`, then we'd get a
compile-time diagnostic instead. They're fairly difficult to read, so I've put one below to guide
you.

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
