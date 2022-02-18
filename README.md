# constexpr-contracts

C++20 was looking good to have contracts until the C++ Standards Committee meeting in Cologne,
Germany of 2019. There, some strong turbulence happened, and the committee ultimately felt that
significantly more work was necessary before contracts are ready for Standard C++.

As one of the authors requesting contracts be removed from the C++20 Working Paper, I'd like to
offer a consolation library, which emulates how I'd like contracts to behave.

## Prerequisites

1. CMake 3.17 or later
2. GCC 11 with libstdc++ or Clang 15 with libc++
3. libLLVM 15

## Installation

### Choosing your toolchain

constexpr-contracts is a bit delicate in which toolchains it supports. If you're using libstdc++ and
don't care for RTTI, then you're in the rare situation where you can install the dependencies, plug
the library in, disable RTTI (if you haven't already), and start using constexpr-contracts.

If you're not in this situation, then your situation is slightly more sticky. Official builds of
LLVM might not be linked against the right standard library ABI for your needs, and it mightn't be
built using RTTI (the [official nightly LLVM releases][apt] are linked against libstdc++'s ABI and
don't have RTTI support).

To work around this problem, [cjdb/llvm-nightly] was created, so that you can have a toolchain that
is compatible with the library. Although llvm-nightly comes with almost the entire LLVM toolchain,
we're only concerned with linking against the right `libLLVM`, so you don't need to transition your
whole toolchain (although this is recommended so that your program can be built with the same
toolchain from top to bottom).

To hook in the right `libLLVM`, we need to provide CMake with
`-DCMAKE_LLVM_ROOT=/path/to/llvm-toolchain/lib/cmake/llvm/LLVMConfig.cmake`.

### With vcpkg

Simply add `constexpr-contracts` to your vcpkg manifest file.

#### Linking in CMake

To link using CMake, you'll need to use `find_package(constexpr-contracts REQUIRED)` to import the
library. To link against the library, use `target_link_libraries(target PRIVATE cjdb::constexpr-contracts)`.

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

Unlike `<cassert>`, these contracts aren't disabled when `NDEBUG` is defined. To disable them, one
needs to define `CJDB_CONTRACTS_DISABLED`.

<table>
	<thead>
		<th style="text-align:center;"><code>CJDB_CONTRACTS_DISABLED</code> defined</th>
		<th style="text-align:center;">Predicate evaluation</th>
		<th style="text-align:center;">Notes</th>
	</thead>
	<tbody style="text-align:center;">
		<tr>
			<td colspan="4"><b>Constant expression</b></td>
		</tr>
		<tr>
			<td>No</td>
			<td><code>false</code></td>
			<td>Program does not compile</td>
		</tr>
		<tr>
			<td>Yes</td>
			<td><code>true</code></td>
			<td>Program compiles</td>
		</tr>
		<tr>
			<td colspan="4"><b>Run-time expression</b></td>
		</tr>
		<tr>
			<td>No</td>
			<td><code>false</code></td>
			<td style="text-align: left;">
				A diagnostic is written to <code>stderr</code> at run-time and then the program crashes.
			</td>
		</tr>
		<tr>
			<td>Yes</td>
			<td><code>false</code></td>
			<td style="text-align: left;">
				Your program is incorrect, but since the macro is disabled, you won't receive any signal
				that this is the case.
			</td>
		</tr>
		<tr>
			<td>No</td>
			<td><code>true</code></td>
			<td style="text-align: left;">
				The program is correct, and won't crash, but it might run a little bit slower. Benchmark to
				see if this is a problem for you before disabling.
			</td>
		</tr>
		<tr>
			<td>Yes</td>
			<td><code>true</code></td>
			<td style="text-align: left;">
				Your program is correct.
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

namespace assert {
	void f(int const argc) noexcept
	{
		CJDB_ASSERT(argc > 1);
	}
} // namespace assert

void g(int const argc) noexcept
{
	assert::f(argc);
}

int main(int argc, char**)
{
	g(argc);
	constexpr auto return_code = 255;
	return argc == 1 ? 0 : return_code;
}
```

The above program will behave as if the assertion is not present in the event `x != 0`. If, however,
`x == 0`, the program will print the following message in debug-mode and then crash.
```
$ /tmp/example
---------------------------------------- Contract report ----------------------------------------
An assumption of 'void assert::f(const int)' is that 'argc > 1', but we provided '1'

------------------------------------------ Stack trace ------------------------------------------
#0 0x0000000000382e1d assert::f(int) /tmp/example.cpp:6:3
#1 0x0000000000382f0b g(int) /tmp/example.cpp:13:1
#2 0x0000000000382f46 main /tmp/example.cpp:18:17
#3 0x00007fc740e3b7ed __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x277ed)
#4 0x00000000002c2efa _start (/tmp/example+0x2c2efa)
Aborted
```

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
$ /tmp/example
---------------------------------------- Contract report ----------------------------------------
A precondition of 'person::person(std::string_view, std::string_view, int)' is that 'age >= 0', but we provided '-1'

------------------------------------------ Stack trace ------------------------------------------
#0 0x00000000003838ec person::person(std::__1::basic_string_view<char, std::__1::char_traits<char> >, std::__1::basic_string_view<char, std::__1::char_traits<char> >, int) /tmp/example.cpp:9:10
#1 0x00000000003833d4 __cxx_global_var_init /tmp/example.cpp:0:0
#2 0x0000000000383485 _GLOBAL__sub_I_example.cpp /tmp/example.cpp:0:0
#3 0x0000000000435db5 __libc_csu_init (/tmp/example+0x435db5)
#4 0x00007f0c2df2977d __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2777d)
#5 0x00000000002c34ba _start (/tmp/example+0x2c34ba)
Aborted
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

## Pitfalls

Because we're peering into the definition here, the code is somewhat brittle: any user-facing
declaration needs to match the definition right down to parameter names; otherwise the contract won't
make much sense to the user.

## (Imagined) FAQs

### Why are the line numbers in the stacktrace a bit off?

Please see llvm-project issue [#53683](https://github.com/llvm/llvm-project/issues/53683).

### Why does this library use `<cstdio>` instead of `<iostream>` or `<format>`?

This library's original primary goal was to serve as a "better assert library" for a standard
library implementation. This means that we can't use the standard library in constexpr-contracts
because it would prevent putting these checks in the very things we use to implement them.

Once the standard library design has been completed, the library will probably be forked, and we'll
move to using `<format>` for the user-facing library so that printing non-standard types becomes
possible.


### Why do I get linker errors for undefined symbols such as `typeinfo for llvm::raw_ostream` or `std::__once_callable`?

It seems like you haven't properly installed the library's dependencies. Please see [Choosing your
toolchain][install].

## Acknowledgements

Thanks to the following people for reviewing!

* @edhebi
* Ben Craig

[apt]: https://apt.llvm.org/
[cjdb/llvm-nightly]: https://github.com/cjdb/llvm-nightly/releases/tag/nightly
[install]: #choosing-your-toolchain
