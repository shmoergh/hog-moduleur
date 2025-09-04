# Todo: Brain SDK Project Setup

## 1. Repository structure
- [x] We're in the top level folder `brain-sdk/`
- [x] Inside it, create these subfolders:
  - [x] `lib/` → reusable libraries (e.g. brain_io, brain_dsp)
  - [x] `programs/` → placeholder for firmware apps (empty for now)
  - [x] `scripts/` → future helper tools (e.g. app generator shell script)
  - [x] `docs/` → todos, design notes

## 2. CMake setup
- [x] In the top-level `CMakeLists.txt`:
  - [x] Set `cmake_minimum_required(VERSION 3.13)`
  - [x] Define project name `brain-sdk`
  - [x] Include `pico_sdk_import.cmake` from `cmake/`
  - [x] Call `pico_sdk_init()`
  - [x] Add `add_subdirectory(lib)` so libraries are always available
  - [x] Add `add_subdirectory(programs)` so apps can be added later
- [x] In `cmake/pico_sdk_import.cmake`:
  - [x] Add the official import logic from the Pico SDK documentation
  - [x] Allow `PICO_SDK_PATH` to be set by environment variable
- [x] Ensure the toolchain is set to **arm-none-eabi-gcc** (the ARM cross-compiler for Pico),
      not the host compiler (e.g., not clang/gcc for macOS/Linux).

## 3. Library scaffolding
- [x] Create `lib/CMakeLists.txt` that:
  - [x] Finds and builds all subdirectories in `lib/`
- [x] Create `lib/brain_io/` folder
- [x] Add `lib/brain_io/CMakeLists.txt` with placeholder:
  ```cmake
  add_library(brain_io INTERFACE)
  target_include_directories(brain_io INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
  ```
- [x] Create `lib/brain_io/include/brain_io/` and add placeholder header `brain_io.h`

## 4. Build system hygiene
- [x] Configure build output to place `.uf2` files in `build/programs/<name>/`
- [x] Ensure build succeeds even if `programs/` is empty (no apps yet)

## 5. Documentation
- [x] Add `docs/README.md` describing folder purpose
- [x] Add `docs/01_project-setup-todo.md` (this file) to track completion
