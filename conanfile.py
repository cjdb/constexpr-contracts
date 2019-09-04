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
    name = "contracts-consolation"
    version = "0.1"
    license = "Apache Licence, Version 2.0"
    author = "Christopher Di Bella <cjdb.ns@gmail.com>"
    url = "https://github.com/cjdb/contracts-consolation"
    description = "A C++20 library supporting compile-time friendly contracts."
    topics = ("c++20", "contracts")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "enable_clang_tidy": ["Off", "On"],
        "clang_tidy_path": "ANY"
    }
    default_options = {
        "enable_clang_tidy": "Off",
        "clang_tidy_path": "/usr/bin/clang-tidy"
    }
    exports_sources = "include/*", "LICENSE", "CMakeLists.txt", "test/*"
    no_copy_source = True

    def build(self):
        cmake = CMake(self)
        cmake.definitions["ENABLE_CLANG_TIDY"] = self.options.enable_clang_tidy
        cmake.definitions["CLANG_TIDY_PATH"] = self.options.clang_tidy_path
        cmake.configure()
        cmake.build()

        env_build = RunEnvironment(self)
        with tools.environment_append(env_build.vars):
            cmake.test(output_on_failure=True)
        cmake.install()

    def package(self):
        self.copy("include/")
        self.copy("LICENSE", dst="licenses", ignore_case=True, keep_path=False)

    def package_id(self):
        self.info.header_only()
