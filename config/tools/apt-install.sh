#!/bin/bash
# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#

set -e



apt_install() {
   echo "Installing system tools"

   DISTRO=`lsb_release -a 2>&1 | egrep 'Distributor:\s+' | cut -d':' -f2 | tr -d '\t'`
   CODENAME=`lsb_release -a 2>&1 | egrep 'Codename:\s+' | cut -d':' -f2 | tr -d '\t'`

   # We need to know who runs non-root commands
   if [[ $1 == "" ]]; then
      echo "$0: ./$0 username"
      echo "`username` should be \"`whoami`\" or \"root\""
      exit 1
   fi

   if [[ $(id -u) -ne 0 ]]; then
      echo "$0: must be run as root"
      exit 1
   fi

   if [[ $DISTRO == 'Ubuntu' ]]; then
      GCC_LATEST_REPO='ppa:ubuntu-toolchain-r/test'
   else
      GCC_LATEST_REPO='deb http://deb.debian.org/debian testing main'
   fi

   apt-get update
   apt-get dist-upgrade -y
   apt-get install -y curl gnupg wget software-properties-common
   wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
   add-apt-repository "deb http://apt.llvm.org/$CODENAME/ llvm-toolchain-$CODENAME main"
   add-apt-repository "${GCC_LATEST_REPO}"
   apt-get update
   apt-get install -y   \
       build-essential  \
       bzip2            \
       clang-format     \
       clang-tidy       \
       clang-tools      \
       clang            \
       clangd           \
       gcc-10           \
       g++-10           \
       gdb              \
       git              \
       gzip             \
       libc++-dev       \
       libc++abi-dev    \
       libclang-dev     \
       lld              \
       lldb             \
       llvm-dev         \
       llvm-runtime     \
       llvm             \
       ninja-build      \
       openssh-server   \
       python3          \
       python3-pip      \
       python3-clang    \
       sed              \
       tar              \
       unzip            \
       zip              \
       zlib1g
   python3 -m pip install pip --upgrade
}

install_cmake() {
   echo "Installing CMake..."
   if [[ $1 == "root" ]]; then
      python3 -m pip install cmake
   else
      sudo -u $1 python3 -m pip install --user cmake
   fi
}

install_vcpkg() {
   git clone https://github.com/microsoft/vcpkg.git
   pushd vcpkg
   cp ../cmake/vcpkg/* triplets/community/.
   ./bootstrap-vcpkg.sh -disableMetrics
   popd
}

if [ ! -f "${PWD}/config/tools/apt-install.sh" ]; then
   >&2 echo "apt-install.sh must be run from the top-level directory of the project."
   exit 1
fi

apt_install
install_cmake $1
install_vcpkg
