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
set(CMAKE_FIND_ROOT_PATH "/usr/")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

string(
   JOIN " " CMAKE_CXX_FLAGS
   "${CMAKE_CXX_FLAGS}"
   -fstack-protector
   -fdiagnostics-color=always
   -pedantic
   -Werror
   -Wall
   -Wextra
   -Wcast-align
   -Wconversion
   -Wdouble-promotion
   -Wnon-virtual-dtor
   -Wold-style-cast
   -Woverloaded-virtual
   -Wshadow
   -Wsign-conversion
   -Wsign-promo
   -Wunused
   -Wformat=2
   -Wodr
   -Wno-attributes
   -Wnull-dereference
   -Wno-unused-command-line-argument
   -fno-rtti
   -fvisibility=hidden
)

string(
   JOIN " " CONSTEXPR_CONTRACTS_FLAGS_DEBUG
   "${CONSTEXPR_CONTRACTS_FLAGS_DEBUG}"
   -fsanitize=address,undefined
   -fstack-protector-strong
)

string(
   JOIN " " CONSTEXPR_CONTRACTS_FLAGS_RELEASE
   "${CONSTEXPR_CONTRACTS_FLAGS_RELEASE}"
   -DCJDB_CONTRACTS_DISABLED
)
