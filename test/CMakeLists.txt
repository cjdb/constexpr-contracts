#
#  Copyright Christopher Di Bella
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
function(build_contract filename volume)
	set(target "${volume}-${filename}")
	add_executable("${target}" "${filename}.cpp")
	target_include_directories("${target}" PRIVATE "${CMAKE_SOURCE_DIR}/include")
	target_compile_definitions("${target}"
	                           PRIVATE CJDB_EXPECTS=CJDB_${volume}_EXPECTS
	                           PRIVATE CJDB_ASSERT=CJDB_${volume}_ASSERT
	                           PRIVATE CJDB_ENSURES=CJDB_${volume}_ENSURES)
endfunction()

build_contract(pass QUIET)
add_test(test.quiet-pass QUIET-pass)

build_contract(pass ALERT)
add_test(test.alert-pass ALERT-pass)

function(test_contract target expected_output)
	add_test(NAME "test.${target}"
	         COMMAND bash
	         "${CMAKE_SOURCE_DIR}/test/check-failure.sh"
	         "${target}"
	         "${expected_output}"
	         WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endfunction()

function(test_quiet_contract target expected_output)
	if(CMAKE_BUILD_TYPE MATCHES "Release")
		test_contract("QUIET-${target}" "")
	else()
		test_contract("QUIET-${target}" "${expected_output}")
	endif()
endfunction()

set(precondition_failure
    "pre-condition \`0 == 1\` failed in \`int main()\` at ../test/fail-expects.cpp:20")

build_contract(fail-expects QUIET)
test_quiet_contract(fail-expects "${precondition_failure}")

build_contract(fail-expects ALERT)
test_contract(ALERT-fail-expects "${precondition_failure}")

###

set(assertion_failure
    "assertion \`not\ \"runtime failure\"` failed in \`int main()\` at ../test/fail-assert.cpp:20")

build_contract(fail-assert QUIET)
test_quiet_contract(fail-assert "${assertion_failure}")

build_contract(fail-assert ALERT)
test_contract(ALERT-fail-assert "${assertion_failure}")

###

set(postcondition_failure
    "post-condition \`x != 0\` failed in \`int main()\` at ../test/fail-ensures.cpp:21")

build_contract(fail-ensures QUIET)
test_quiet_contract(fail-ensures "${postcondition_failure}")

build_contract(fail-ensures ALERT)
test_contract(ALERT-fail-ensures "${postcondition_failure}")
