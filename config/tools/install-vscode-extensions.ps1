$vscode_path = Get-Command code
If ([string]::IsNullOrEmpty($vscode_path)) {
   'Visual Studio Code not found. Please download and install Visual Studio Code from https://code.visualstudio.com/download.'
   exit 1
}

code --install-extension Equinusocio.vsc-material-theme        && \
code --install-extension helgardrichard.helium-icon-theme      && \
code --install-extension j-zeppenfeld.tab-indent-space-align   && \
code --install-extension llvm-vs-code-extensions.vscode-clangd && \
code --install-extension ms-vscode-remote.remote-wsl           && \
code --install-extension ms-vscode-remote.remote-ssh           && \
code --install-extension ms-vscode.cmake-tools                 && \
code --install-extension twxs.cmake                            && \
code --install-extension vadimcn.vscode-lldb
