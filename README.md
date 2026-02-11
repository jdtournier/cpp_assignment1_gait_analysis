[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/jdtournier/cpp_assignment1_gait_analysis?quickstart=1)


# Template codespace for use on KCL BEng C++ module

- contains environment with Ubuntu 24.04 LTS, g++ with C++20.
- provides [the `build` script](https://github.com/jdtournier/simple_build) as simple zero-config C++ build system. This is installed in `/usr/local/bin`.
- provides `zip_all` script to find and zip up all C++ code files for submission of assignments (this means any file with suffix `.cpp`, `.cc`, `.h` or `.hpp`). This is also installed in `/usr/local/bin`.
- provides the `termviz.h` header for simple plotting and image display in the terminal<br>
  This is provided by [the terminal_graphics library](https://github.com/jdtournier/terminal_graphics/)). It relies on the sixel protocol, and requires the `terminal.integrated.enableImages` VSCode setting to be enabled (which is the case for this codespace). This is installed in `/usr/local/include`, which is already included in the compiler default search path.
