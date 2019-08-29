# Contracts Consolation

C++20 was looking good to have contracts until the C++ Standards Committee meeting in Cologne, Germany
of 2019. There, some strong turbulence happened, and the committee ultimately felt that significantly
more work was necessary before contracts are ready for Standard C++.

As one of the authors requesting contracts be removed from the C++20 Working Paper, I'd like to offer
a consolation library, which emulates how I'd like contracts to behave.

## Prerequisites

1. CMake 3.14 or later
2. Conan 1.18 or later (if using Conan)
3. A compiler that supports `consteval` in C++20-mode (e.g. Clang 9).
4. A standard library that supports `std::is_constant_evaluated` in C++20-mode (e.g. libc++ 9)

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
		<th>Alerting contracts</th>
		<th>Quiet contracts</th>
	</thead>
	<tbody>
		<tr>
			<td>Pre-condition</td>
			<td><code>CJDB_ALERT_EXPECTS</code></td>
			<td><code>CJDB_QUIET_EXPECTS</code></td>
		</tr>
		<tr>
			<td>Assertion</td>
			<td><code>CJDB_ALERT_ASSERT</code></td>
			<td><code>CJDB_QUIET_ASSERT</code></td>
		</tr>
		<tr>
			<td>Post-condition</td>
			<td><code>CJDB_ALERT_ENSURES</code></td>
			<td><code>CJDB_QUIET_ENSURES</code></td>
		</tr>
	</tbody>
</table>

Alerting contracts will always generate a message when a contract is violated, and then crash. Quiet
contracts, on the other hand, will generate a message in debug mode and then crash; when `NDEBUG` is
defined, they will still crash when a contract is violated, but the code for providing a message isn't
generated.

Note that unlike `assert`, all contracts will terminate the program when a contract is violated.
Although that involves the check being present in release builds, it _can_ sometimes lead to more
aggressive optimisations, since the optimiser will have stronger confidence about what code can be
reached with particular values.

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
	CJDB_ALERT_ASSERT(x != 0);
	std::cout << (5 / x) << '\n';
}
```

The above program will behave as if the assertion is not present in the event `x != 0`. If, however,
`x == 0`, the program will unconditionally print the following message and then abort.
```
pre-condition `x != 0` failed in `int main()` at ./example1.cpp:5
```

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
	, age_{(CJDB_QUIET_EXPECTS(age >= 0), age)} // parens are necessary!
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
pre-condition `age >= 0` failed in `person::person(const std::string_view, const std::string_view, const int)` at ./example2.cpp:7
```

This diagnostic won't always appear. Any build that defines `NDEBUG` will suppress the diagnostic, but
the crash will _always_ happen.

### Post-conditions

Finally, post-conditions allow us to provide a guarantee that our invariants hold upon exiting a
function. These are a bit different in usage: instead of providing just an expression, the intended
usage is to place your post-condition in a return-statement. As such, there should only be one
post-condition per return-statement in a function.

```cpp
int f()
{
	auto result = g();
	return CJDB_ALERT_ENSURES(result >= 0), result;
}
```

This usage lets us chain post-conditions before our result is returned. As with pre-conditions and
assertions, the diagnostic indicates what kind of contract was violated.

## Contracts Consolation in constant expressions

Notice that `person::person` is `constexpr`. Contracts Consolation has a huge advantage over both
`assert` and `static_assert`, because it _can_ be evaluated at compile-time or at run-time, depending
on the context. If we wanted to constant-initialise `ada_lovelace`, then we'd get a compile-time
diagnostic instead. They're fairly difficult to read, so I've put one below to guide you.

```
../test/person.cpp:16:16: error: constexpr variable 'ada_lovelace' must be initialized by a constant expression
constexpr auto ada_lovelace = person{
               ^              ~~~~~~~
../test/person.cpp:8:10: note: non-constexpr function 'abort' cannot be used in a constant expression
        , age_{(CJDB_QUIET_EXPECTS(age >= 0), age)}
                ^
../include/cjdb/contracts.hpp:35:33: note: expanded from macro 'CJDB_QUIET_EXPECTS'
#define CJDB_QUIET_EXPECTS(...) CJDB_EXPECTS_IMPL(::cjdb::coco_detail::contract_mode::quiet, __VA_ARGS__)
                                ^
../include/cjdb/contracts.hpp:47:2: note: expanded from macro 'CJDB_EXPECTS_IMPL'
        CJDB_CONTRACT_IMPL("pre-condition", CJDB_CONTRACT_MODE, __VA_ARGS__)
        ^
../include/cjdb/contracts.hpp:68:3: note: expanded from macro 'CJDB_CONTRACT_IMPL'
                std::abort();                                                                                                                          \
                ^
../test/person.cpp:8:10: note: in call to '&fail->operator()(false)'
../include/cjdb/contracts.hpp:35:33: note: expanded from macro 'CJDB_QUIET_EXPECTS'
#define CJDB_QUIET_EXPECTS(...) CJDB_EXPECTS_IMPL(::cjdb::coco_detail::contract_mode::quiet, __VA_ARGS__)
                                ^
../include/cjdb/contracts.hpp:47:2: note: expanded from macro 'CJDB_EXPECTS_IMPL'
        CJDB_CONTRACT_IMPL("pre-condition", CJDB_CONTRACT_MODE, __VA_ARGS__)
        ^
../include/cjdb/contracts.hpp:72:4: note: expanded from macro 'CJDB_CONTRACT_IMPL'
                        fail(false);                                                                                                                        \
                        ^
../test/person.cpp:8:10: note: in call to '&[](const bool result) {
    auto fail = [](const bool result) noexcept {
        if (result) {
            return true;
        }
        std::abort();
    };
    if (__builtin_expect(!result, false)) {
        if (std::is_constant_evaluated()) {
            fail(false);
        }
        return false;
    }
    return true;
}->operator()(false)'
../include/cjdb/contracts.hpp:35:33: note: expanded from macro 'CJDB_QUIET_EXPECTS'
#define CJDB_QUIET_EXPECTS(...) CJDB_EXPECTS_IMPL(::cjdb::coco_detail::contract_mode::quiet, __VA_ARGS__)
                                ^
../include/cjdb/contracts.hpp:47:2: note: expanded from macro 'CJDB_EXPECTS_IMPL'
        CJDB_CONTRACT_IMPL("pre-condition", CJDB_CONTRACT_MODE, __VA_ARGS__)
        ^
../include/cjdb/contracts.hpp:63:71: note: expanded from macro 'CJDB_CONTRACT_IMPL'
#define CJDB_CONTRACT_IMPL(CJDB_KIND, CJDB_CONTRACT_MODE, ...) (void)([](bool const result) constexpr {                                      \
                                                                      ^
../test/person.cpp:16:31: note: in call to 'person({&"Ada"[0], 3}, {&"Lovelace"[0], 8}, -1)'
constexpr auto ada_lovelace = person{
                              ^
/usr/include/stdlib.h:588:13: note: declared here
extern void abort (void) __THROW __attribute__ ((__noreturn__));
            ^
1 error generated.
```

That's a lot of error to say that `age < 0`! We can fortunately ignore 90% of the above. The only
useful information in the entire diagnostic for our purposes are the first six lines.

```
../test/person.cpp:16:16: error: constexpr variable 'ada_lovelace' must be initialized by a constant expression
constexpr auto ada_lovelace = person{
               ^              ~~~~~~~
../test/person.cpp:8:10: note: non-constexpr function 'abort' cannot be used in a constant expression
        , age_{(CJDB_QUIET_EXPECTS(age >= 0), age)}
                ^
```

Here, we see that `constexpr auto ada_lovelace = person{` is the offending call-site, and that
`CJDB_QUIET_EXPECTS(age >= 0)` is the contract that we broke. Literally everything else is helper
information that the compiler needs to give us, but we don't need.

Because we're peering into the definition here, the code is somewhat brittle: any user-facing
declaration needs to match the definition right down to parameter names; otherwise the contract won't
make much sense to the user.

## Removing Contracts Consolation

If you're designing a library and don't want Contracts Consolation to intrude on your users' code,
then you can include `"cjdb/contracts_undef.hpp"`, which will undefine all the macros, including the
header guards (so your users can also include Contracts Collaboration at their discretion).
