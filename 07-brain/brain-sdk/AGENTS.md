# Repository Guidelines

## Project Structure & Modules
- `lib/` reusable libraries: `brain-io`, `brain-ui`, `brain-common` (public headers under `lib/<mod>/include/<mod>/`). Example: `lib/brain-ui/include/brain-ui/button.h`.
- `programs/` firmware apps (one folder per app). Example: `programs/hardware-test/` builds `hardware-test`.
- `pico-sdk/` Pico SDK (git submodule). `build/` CMake output. Docs live in `docs/`.

## Build, Flash, and Dev Commands
- Configure and build:
  - `cmake -B build -G "Unix Makefiles"` (add flags like `-DPICO_BOARD=pico2` if needed)
  - `cmake --build build -j`
- Artifacts: `build/programs/<name>/<name>.{elf,uf2}`. Flash by holding BOOTSEL, mount `RPI-RP2`, then copy the `.uf2`.
- Scaffold a new app: `./scripts/new-program.sh <name>` then reconfigure/rebuild.

## Coding Style & Naming
- C/C++ formatted by `.clang-format` (Google base):
  - Tabs only (width 4), column limit 100, sorted/regrouped includes, left-aligned pointers.
  - Format: `clang-format -style=file -i path/to/file.cpp`.
- File layout: keep public headers in `include/<module>/…`, implementation in `.cpp`. Keep target name = folder name (e.g., `programs/foo/foo.cpp` → `foo`).

## Testing Guidelines
- No formal unit test framework yet. Use `programs/hardware-test/` or create small program-level test apps under `programs/<feature>/` (e.g., `*_test.cpp`) to validate modules on hardware.
- Prefer deterministic, minimal examples. Document expected behavior in `docs/` when adding new modules.

## Commit & Pull Request Guidelines
- Commits: short, imperative, and scoped. Examples:
  - "Add UART processing in MIDI class"
  - "Update pulse to use global GPIO settings"
- PRs: include a clear description, affected modules, build output path(s), and any hardware steps to verify. Link related docs updates/screenshots or serial logs when relevant. One feature/fix per PR where possible.

## Security & Configuration Tips
- Ensure the Pico SDK submodule is initialized: `git submodule update --init --recursive`.
- Toolchain requirements and recommended VSCode settings are in `docs/SETUP.md`. Use OpenOCD/Picoprobe for debugging; set `executable` to the desired `.elf` in `.vscode/launch.json`.

