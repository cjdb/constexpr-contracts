# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
from conans import ConanFile, CMake, RunEnvironment, tools
import os


class ContractsConan(ConanFile):
    name = "constexpr-contracts"
    version = "head"
    license = "Apache-2.0 WITH LLVM-exception"
    author = "Christopher Di Bella <cjdb.ns@gmail.com>"
    url = "https://github.com/cjdb/cjdb-contracts.git"
    description = "A C++20 library supporting compile-time friendly contracts."
    topics = ("cpp", "c++20", "contracts")
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
        self.copy("LICENSE.md", dst="licenses", ignore_case=True, keep_path=False)

    def package_id(self):
        self.info.header_only()
