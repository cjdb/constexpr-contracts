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
from conans import ConanFile, CMake, RunEnvironment, tools
import os


class ContractsConsolationConan(ConanFile):
    name = "cjdb.contracts"
    version = "1.0"
    license = "Apache 2.0"
    author = "Christopher Di Bella <cjdb.ns@gmail.com>"
    url = "https://github.com/cjdb/contracts-consolation"
    description = "Contracts Consolation is a C++20 library supporting compile-time friendly contracts."
    topics = ("c++20", "contracts")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "enable_clang_tidy": ["Off", "On"],
        "clang_tidy_path": "ANY"
    }
    generators = "cmake_paths", "cmake"

    def source(self):
        self.run("git clone https://github.com/cjdb/contracts-consolation.git")
        # This small hack might be useful to guarantee proper /MT /MD linkage
        # in MSVC if the packaged project doesn't have variables to set it
        # properly
        tools.replace_in_file("contracts-consolation/CMakeLists.txt", "PROJECT(contracts-consolation)",
                              '''PROJECT(contracts-consolation)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()''')

    def build(self):
        cmake = CMake(self)
        cmake.definitions["ENABLE_CLANG_TIDY"] = self.options.enable_clang_tidy
        cmake.definitions["CLANG_TIDY_PATH"] = self.options.clang_tidy_path
        cmake.configure(source_folder="")

        cmake.build()

        env_build = RunEnvironment(self)
        with tools.environment_append(env_build.vars):
            cmake.test(output_on_failure=True)

    def package(self):
        self.copy("*.hpp", dst="include", src="include")

    def package_info(self):
        self.cpp_info.libs = ["cjdb.contracts"]
