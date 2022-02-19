# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0
#
macro(PROJECT_TEMPLATE_EXTRACT_ADD_TARGET_ARGS optional_values single_value_args multi_value_args)
	set(
	   optional_values2
	   "${optional_values}"
	)
	set(
	   single_value_args2
	   "${single_value_args}"
	   TARGET
	   FILENAME
	)
	set(
	   multi_value_args2
	   "${multi_value_args}"
	   FILENAMES
	   COMPILE_OPTIONS
	   INCLUDE
	   DEFINITIONS
	   LINK_OPTIONS
	   LINK_TARGETS
	)
	cmake_parse_arguments(
	   add_target_args
	   "${optional_values2}"
	   "${single_value_args2}"
	   "${multi_value_args2}"
	   ${ARGN}
	)

	if("${add_target_args_TARGET}" STREQUAL "")
		message(FATAL_ERROR "TARGET is not set. Cannot build a target without naming it!")
	endif()

	if(add_target_args_FILENAME AND add_target_args_FILENAMES)
		message(FATAL_ERROR "FILENAME and FILENAMES can't both be set.")
	elseif(NOT add_target_args_FILENAME AND NOT add_target_args_FILENAMES)
		message(FATAL_ERROR "Neither FILENAME nor FILENAMES are set. Cannot build target ${add_target_args_TARGET} without any files to build.")
	endif()
endmacro()

function(cxx_binary)
	set(optional_args "")
	set(single_args "")
	set(multi_args "")
	PROJECT_TEMPLATE_EXTRACT_ADD_TARGET_ARGS(
	   "${optional_args}"
	   "${single_args}"
	   "${multi_args}"
	   ${ARGN}
	)

	add_executable("${add_target_args_TARGET}" "${add_target_args_FILENAME}" "${add_target_args_FILENAMES}")
	target_include_directories("${add_target_args_TARGET}" PUBLIC "${PROJECT_SOURCE_DIR}/include")

	target_compile_definitions("${add_target_args_TARGET}" PRIVATE "${add_target_args_DEFINITIONS}")
	target_include_directories("${add_target_args_TARGET}" PRIVATE "${add_target_args_INCLUDE}")
	target_compile_options("${add_target_args_TARGET}" PRIVATE "${add_target_args_COMPILE_OPTIONS}")
	target_link_options("${add_target_args_TARGET}" PRIVATE "${add_target_args_LINK_OPTIONS}")
	target_link_libraries("${add_target_args_TARGET}" PRIVATE "${add_target_args_LINK_TARGETS}")
endfunction()

function(cxx_library)
	set(optional_args "")
	set(single_args LIBRARY_TYPE)
	set(multi_args DEFINE_AND_EXPORT INCLUDE_AND_EXPORT LINK_AND_EXPORT)
	PROJECT_TEMPLATE_EXTRACT_ADD_TARGET_ARGS(
	   "${optional_args}"
	   "${single_args}"
	   "${multi_args}"
	   ${ARGN}
	)

	set(legal_library_types "" STATIC SHARED MODULE OBJECT)
	list(FIND legal_library_types "${add_target_args_LIBRARY_TYPE}" library_type_result)
	if(library_type_result EQUAL -1)
		message(FATAL_ERROR "Cannot add a library of type \"${add_target_args_LIBRARY_TYPE}\"")
	endif()

	add_library("${add_target_args_TARGET}" ${add_target_args_LIBRARY_TYPE} "${add_target_args_FILENAME}" "${add_target_args_FILENAMES}")
	target_include_directories(
	  "${add_target_args_TARGET}" PUBLIC
	  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)
  target_include_directories(
      "${add_target_args_TARGET}" SYSTEM PUBLIC
      $<BUILD_INTERFACE:${add_target_args_INCLUDE_AND_EXPORT}>
      $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${add_target_args_INCLUDE_AND_EXPORT}>)

	target_compile_definitions("${add_target_args_TARGET}" PRIVATE "${add_target_args_DEFINITIONS}")
	target_include_directories("${add_target_args_TARGET}" SYSTEM PRIVATE "${add_target_args_INCLUDE}")
	target_compile_options("${add_target_args_TARGET}" PRIVATE "${add_target_args_COMPILE_OPTIONS}")
	target_link_options("${add_target_args_TARGET}" PRIVATE "${add_target_args_LINK_OPTIONS}")
	target_link_libraries("${add_target_args_TARGET}" PRIVATE "${add_target_args_LINK_TARGETS}")
	target_link_libraries("${add_target_args_TARGET}" PUBLIC
	  "$<BUILD_INTERFACE:${add_target_args_LINK_AND_EXPORT}>"
	  "$<INSTALL_INTERFACE:${add_target_args_LINK_AND_EXPORT}>")
endfunction()

function(cxx_test)
	cxx_binary(${ARGN})

	set(optional_args "")
	set(single_args TEST_FRAMEWORK)
	set(multi_args "")
	PROJECT_TEMPLATE_EXTRACT_ADD_TARGET_ARGS(
	   "${optional_args}"
	   "${single_args}"
	   "${multi_args}"
	   ${ARGN}
	)

	target_link_libraries(
	   "${add_target_args_TARGET}"
	   PRIVATE
	   ${${PROJECT_NAME}_TEST_FRAMEWORK}
	   $<$<BOOL:${${PROJECT_NAME}_NEEDS_TEST_MAIN}>:test_main>
	)

	add_test("test.${add_target_args_TARGET}" "${add_target_args_TARGET}")
endfunction()

set(PROJECT_TEMPLATE_ALERT_MISSING_BENCHMARK_FRAMEWORK Off)
function(cxx_benchmark)
	cxx_binary(${ARGN})

	set(optional_args "")
	set(single_args BENCHMARK_FRAMEWORK)
	set(multi_args "")
	PROJECT_TEMPLATE_EXTRACT_ADD_TARGET_ARGS(
	   "${optional_args}"
	   "${single_args}"
	   "${multi_args}"
	   ${ARGN}
	)

	target_link_libraries(
	   "${add_target_args_TARGET}"
	   PRIVATE
	   benchmark::benchmark
	   benchmark::benchmark_main
	)
endfunction()
