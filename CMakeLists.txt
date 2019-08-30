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
cmake_minimum_required(VERSION 3.14)
project("cjdb.contracts" CXX)
add_library(${PROJECT_NAME} INTERFACE)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_CLANG_TIDY "Enables building with clang-tidy" Off)
set(CLANG_TIDY_PATH "/usr/bin/clang-tidy" CACHE
   STRING "Sets the path for clang-tidy. Defaults to \"/usr/bin/clang-tidy\".")

if(${ENABLE_CLANG_TIDY})
	set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_PATH}" "-p=${CMAKE_BINARY_DIR}")
endif()

enable_testing()
include(CTest)

add_subdirectory(test)
