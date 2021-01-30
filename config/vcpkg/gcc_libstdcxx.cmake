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
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

set(VCPKG_CMAKE_SYSTEM_NAME Linux)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

string(
   JOIN " " CMAKE_CXX_FLAGS
   "${CMAKE_CXX_FLAGS}"
   -fvisibility=hidden
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
)

string(
   JOIN " " CMAKE_CXX_FLAGS_DEBUG
   "${CMAKE_CXX_FLAGS_DEBUG}"
   -fsanitize=address,undefined
   -fstack-protector-strong
)

set(CMAKE_CXX_COMPILER "g++")
set(PROJECT_TEMPLATE_CXX_COMPILER_MINIMUM_VERSION 10)

set(CMAKE_AR "ar")
set(CMAKE_RANLIB "ranlib")

string(
   JOIN " " CMAKE_EXE_LINKER_FLAGS
   "${CMAKE_EXE_LINKER_FLAGS}"
   -fuse-ld=gold
)
