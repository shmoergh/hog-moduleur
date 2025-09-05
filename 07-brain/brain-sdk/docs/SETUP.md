# Brain SDK – Development Setup Guide

## Basic folder structure
```
brain-sdk/
├── ai/            # Project rules, todos, and documentation
├── build/         # CMake build output
├── cmake/         # CMake helper scripts
├── docs/          # Documentation and conventions
├── lib/           # Reusable libraries (e.g. brain-io, brain-ui)
├── pico-sdk/      # Pico SDK (as a git submodule)
├── programs/      # Firmware applications
├── scripts/       # Helper scripts (e.g. new-program.sh)
```

## Prerequisites
- XCode (for build tools on macOS)
- VSCode (recommended editor)
- Homebrew (for package management)
- CMake (build system)
- GNU ARM Embedded Toolchain (`arm-none-eabi-gcc`)
- OpenOCD (for debugging with Picoprobe)
- Minicom (optional, for serial console)

## Installing the Pico SDK as a submodule
Run these commands in your project root:
```sh
git submodule add https://github.com/raspberrypi/pico-sdk.git pico-sdk
git submodule update --init --recursive
```
If the folder already exists, remove it from git tracking first:
```sh
git rm -r --cached pico-sdk
rm -rf pico-sdk
```
Then add as a submodule as above.

## Recommended VSCode and clang-format settings for C/C++

Add the following to your VSCode `settings.json` for consistent tab-based formatting in C and C++:

```jsonc
// All for C
"C_Cpp.clang_format_style": "file",
"C_Cpp.clang_format_fallbackStyle": "none",
"[c]": {
	"editor.insertSpaces": false,
	"editor.tabSize": 4,
	"editor.detectIndentation": false,
	"editor.formatOnSave": true,
	"editor.defaultFormatter": "xaver.clang-format"
},
"[cpp]": {
	"editor.insertSpaces": false,
	"editor.tabSize": 4,
	"editor.detectIndentation": false,
	"editor.formatOnSave": true,
	"editor.defaultFormatter": "xaver.clang-format"
},
```

This ensures that both VSCode and clang-format use tabs (width 4) for indentation in all C/C++ files.

## Creating a new program
Use the helper script:
```sh
./scripts/new-program.sh <program-name>
```
This will create a new folder under `programs/` with boilerplate files. After running, re-run CMake configure/build:
```sh
rm -rf build
cmake -B build -G "Unix Makefiles"
cmake --build build
```


## C++ Linting & Formatting
To enforce code style and formatting (Google C++ Style, 4-space indent, etc.), use clang-format:

### Install clang-format
```sh
brew install clang-format
```

### Usage
- Format a file in-place:
	```sh
	clang-format -style=file -i path/to/file.cpp
	```
- Or use the Clang-Format VSCode extension for auto-formatting on save.

The `.clang-format` file in the project root configures formatting rules.

## Debugging a program
Edit `.vscode/launch.json` and set the `executable` parameter to the desired program's `.elf` file, e.g.:
```jsonc
"executable": "${workspaceFolder}/build/programs/blinky/blinky.elf"
```
Select the debug configuration in VSCode and hit F5 to start debugging with Picoprobe.
