# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
function(build_contract filename)
	set(target "${filename}")
	set(target_ios "${filename}_ios")
	add_executable("${target}" "${filename}.cpp")
	add_executable("${target_ios}" "${filename}.cpp")
	if(MSVC)
		target_compile_options("${target}" PRIVATE "/permissive-")
		target_compile_options("${target_ios}" PRIVATE "/permissive-" "/DCJDB_USE_IOSTREAM")
	else()
		target_compile_options("${target_ios}" PRIVATE "-DCJDB_USE_IOSTREAM")
	endif()
	target_include_directories("${target}" PRIVATE "${CMAKE_SOURCE_DIR}/include")
	target_include_directories("${target_ios}" PRIVATE "${CMAKE_SOURCE_DIR}/include")
endfunction()

build_contract(pass)
add_test(test.pass pass)
add_test(test.pass_ios pass_ios)

function(test_contract target expected_output)
	set(args "${CMAKE_SOURCE_DIR}/test/check-failure.py"
	          "--process-name=${target}"
	          "--expected-output=${expected_output}")
	if (${CMAKE_BUILD_TYPE} MATCHES ".*Debug.*")
		list(APPEND args "--debug=True")
	endif()
	add_test(NAME "test.${target}"
	         COMMAND python3 ${args}
	         WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
	list(TRANSFORM args APPEND "_ios" AT 1)
	add_test(NAME "test.${target}_ios"
	         COMMAND python3 ${args}
	         WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endfunction()

function(test_quiet_contract target expected_output)
	if(CMAKE_BUILD_TYPE MATCHES "Release")
		test_contract("${target}" "")
	else()
		test_contract("${target}" "${expected_output}")
	endif()
endfunction()


build_contract(fail-expects)
test_quiet_contract("fail-expects"
   "^[.]{2}/[.]{2}/[.]{2}/test/fail-expects[.]cpp:22: pre-condition \`argc != 1\` failed in \`int main(int, char\\s*[*]{2})\`$")

build_contract(fail-assert)
test_quiet_contract("fail-assert"
   "[.]{2}/[.]{2}/[.]{2}/test/fail-assert[.]cpp:20: assertion \`argc > 1\` failed in \`int main(int, char\\s*[*]{2})\`$")

build_contract(fail-ensures)
test_quiet_contract("fail-ensures"
   "../../../test/fail-ensures.cpp:22: post-condition \`x > 1\` failed in \`int magic(int)\`\n")
