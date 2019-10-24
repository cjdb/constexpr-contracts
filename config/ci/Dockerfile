# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
FROM ubuntu:disco

RUN apt update
RUN apt upgrade -y
RUN apt install -y software-properties-common
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 15CF4D18AF4F7421
RUN add-apt-repository 'deb http://apt.llvm.org/disco/ llvm-toolchain-disco-9 main'
RUN apt update
RUN apt install -y gcc-9 g++-9 binutils ninja-build valgrind
RUN apt install -y clang-9 clang-tools-9 clang-tidy-9 lld-9 libc++-9-dev libc++abi-9-dev
RUN apt install -y python3 python3-pip
RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install cmake conan

RUN cd /tmp                               && \
    conan new hello/0.1 -t                && \
    conan install .                       && \
    cd /                                  && \
    rm -rf /tmp/*
RUN sed -i '69s/"8"/"8", "9"/' ${HOME}/.conan/settings.yml
