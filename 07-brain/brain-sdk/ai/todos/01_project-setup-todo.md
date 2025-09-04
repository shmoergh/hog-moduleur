# Todo: Brain SDK Project Setup

## 1. Repository structure
- [ ] We're in the top level folder `brain-sdk/`
- [ ] Inside it, create these subfolders:
  - [ ] `lib/` → reusable libraries (e.g. brain_io, brain_dsp)
  - [ ] `programs/` → placeholder for firmware apps (empty for now)
  - [ ] `scripts/` → future helper tools (e.g. app generator shell script)
  - [ ] `docs/` → todos, design notes

## 2. CMake setup
- [ ] In the top-level `CMakeLists.txt`:
  - [ ] Set `cmake_minimum_required(VERSION 3.13)`
  - [ ] Define project name `brain-sdk`
  - [ ] Include `pico_sdk_import.cmake` from `cmake/`
  - [ ] Call `pico_sdk_init()`
  - [ ] Add `add_subdirectory(lib)` so libraries are always available
  - [ ] Add `add_subdirectory(programs)` so apps can be added later
- [ ] In `cmake/pico_sdk_import.cmake`:
  - [ ] Add the official import logic from the Pico SDK documentation
  - [ ] Allow `PICO_SDK_PATH` to be set by environment variable
- [ ] Ensure the toolchain is set to **arm-none-eabi-gcc** (the ARM cross-compiler for Pico),
      not the host compiler (e.g., not clang/gcc for macOS/Linux).

## 3. Library scaffolding
- [ ] Create `lib/CMakeLists.txt` that:
  - [ ] Finds and builds all subdirectories in `lib/`
- [ ] Create `lib/brain_io/` folder
- [ ] Add `lib/brain_io/CMakeLists.txt` with placeholder:
  ```cmake
  add_library(brain_io INTERFACE)
  target_include_directories(brain_io INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
  ```
- [ ] Create `lib/brain_io/include/brain_io/` and add placeholder header `brain_io.h`

## 4. Build system hygiene
- [ ] Configure build output to place `.uf2` files in `build/programs/<name>/`
- [ ] Ensure build succeeds even if `programs/` is empty (no apps yet)

## 5. Documentation
- [ ] Add `docs/README.md` describing folder purpose
- [ ] Add `docs/01_project-setup-todo.md` (this file) to track completion
