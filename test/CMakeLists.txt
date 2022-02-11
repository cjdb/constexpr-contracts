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


cxx_test(TARGET pass FILENAME pass.cpp LINK_TARGETS contracts ${llvm_libs})

set(macros equal_to not_equal_to less less_equal greater_equal greater)

foreach(i IN ITEMS ${macros})
  cxx_binary(
    TARGET fail-expects-${i}
    FILENAME fail-expects.cpp
    LINK_TARGETS contracts
    DEFINITIONS TEST=test_kind::${i}
  )
  test_quiet_contract("fail-expects-${i}" "${CMAKE_CURRENT_SOURCE_DIR}/expects")

  cxx_binary(
    TARGET fail-assert-${i}
    FILENAME fail-assert.cpp
    LINK_TARGETS contracts
    DEFINITIONS TEST=test_kind::${i}
  )
  test_quiet_contract("fail-assert-${i}" "${CMAKE_CURRENT_SOURCE_DIR}/assert")

  cxx_binary(
    TARGET fail-ensures-${i}
    FILENAME fail-ensures.cpp
    LINK_TARGETS contracts
    DEFINITIONS TEST=test_kind::${i}
  )
  test_quiet_contract("fail-ensures-${i}" "${CMAKE_CURRENT_SOURCE_DIR}/ensures")
endforeach()
