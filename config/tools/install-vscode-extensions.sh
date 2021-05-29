#!/bin/bash
# Copyright (c) Christopher Di Bella.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#

set -e

if [[ "`which code`" == "" ]]; then
   echo "Couldn't find VS Code."
   echo "If you're using Linux proper, you can install it by running config/tools/install-vscode.sh."
   echo "If you're using Windows, you'll need to download and install Visual Studio Code from https://code.visualstudio.com/download."
   exit 1
fi

code --install-extension Equinusocio.vsc-material-theme
code --install-extension helgardrichard.helium-icon-theme
code --install-extension j-zeppenfeld.tab-indent-space-align
code --install-extension llvm-vs-code-extensions.vscode-clangd
code --install-extension ms-vscode.cmake-tools
code --install-extension twxs.cmake
code --install-extension vadimcn.vscode-lldb
