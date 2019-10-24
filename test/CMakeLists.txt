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
function(build_contract filename)
	set(target "${filename}")
	add_executable("${target}" "${filename}.cpp")
	target_include_directories("${target}" PRIVATE "${CMAKE_SOURCE_DIR}/include")
endfunction()

build_contract(pass)
add_test(test.pass pass)

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
