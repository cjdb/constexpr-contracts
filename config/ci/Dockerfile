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
FROM ubuntu:disco

RUN apt update
RUN apt upgrade -y
RUN apt install -y software-properties-common
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 15CF4D18AF4F7421
RUN add-apt-repository 'deb http://apt.llvm.org/disco/ llvm-toolchain-disco-9 main'
RUN apt update

RUN apt install -y gcc-9 g++-9 binutils ninja-build
RUN apt install -y gdb valgrind
RUN apt install -y clang-9 clang-tools-9 clang-tidy-9 lld-9 libc++-9-dev libc++abi-9-dev
RUN apt install -y python3 python3-pip

RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install cmake conan
