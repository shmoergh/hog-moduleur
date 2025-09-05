# Brain SDK Overview

The Brain SDK is a modular firmware and library collection for the Raspberry Pi Pico, designed to accelerate embedded development. It provides reusable components for I/O, UI, DSP, and more, along with example programs and scripts to streamline project setup and prototyping.

## Features
- Modular libraries for common embedded tasks
- Example programs and scripts for rapid development
- Follows Google C++ Style Guide with embedded-friendly tweaks
- Ready for debugging with Picoprobe and VSCode

## Getting Started
See [docs/SETUP.md](docs/SETUP.md) for setup instructions, prerequisites, and workflow details.

## Folder Structure
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
