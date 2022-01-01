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

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_AR "llvm-ar")
set(CMAKE_RC_COMPILER "llvm-rc")
set(CMAKE_RANLIB "llvm-ranlib")

string(
   JOIN " " CMAKE_CXX_FLAGS
   "${CMAKE_CXX_FLAGS}"
   -stdlib=libstdc++
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

string(
   JOIN " " CMAKE_CXX_FLAGS_RELEASE
   "${CMAKE_CXX_FLAGS_RELEASE}"
   -fsanitize=cfi
   -fno-sanitize=cfi-unrelated-cast # TODO(cjdb): remove once Catch2 properly supports cfi
   -march=x86-64-v4
   -ffast-math
   -flto=thin
)

string(
   JOIN " " CMAKE_EXE_LINKER_FLAGS
   "${CMAKE_EXE_LINKER_FLAGS}"
   -fuse-ld=lld
   -flto=thin
)
