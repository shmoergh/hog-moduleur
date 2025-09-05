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

## Debugging a program
Edit `.vscode/launch.json` and set the `executable` parameter to the desired program's `.elf` file, e.g.:
```jsonc
"executable": "${workspaceFolder}/build/programs/blinky/blinky.elf"
```
Select the debug configuration in VSCode and hit F5 to start debugging with Picoprobe.
