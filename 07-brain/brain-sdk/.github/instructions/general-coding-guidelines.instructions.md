---
applyTo: '**'
---
# Project General Coding Guidelines

## TL;DR Checklist
- 4-space indent, max 100 cols, UTF-8, Unix newlines.
- Filenames: `lower_case_with_underscores.{h,cc,cpp}`.
- Folder names: `kebab-case` (e.g., `brain-io`, `brain-dsp`).
- Types/classes/enums: `PascalCase`.
- Functions: `PascalCase`.
- Variables: `lower_case_with_underscores`; class members end with `_`.
- Constants: `kPascalCase`.
- Macros: `ALL_CAPS_WITH_UNDERSCORES` (avoid; prefer `constexpr`/`enum`).
- No exceptions, no RTTI by default. No dynamic allocation in ISRs.
- `const` and `constexpr` aggressively. Prefer `enum class` over `#define`.
- One `#include` per line. Order: C system → C++ std → third-party → project.
- Headers are self-contained & guarded. `.cc/.cpp` mirrors header order.
- Public APIs documented with brief Doxygen comments.


## CMake Conventions
- Targets mirror file names: `brain-io`, `adc-sampler`, etc.
- Libraries default to **`INTERFACE`** if header-only; otherwise `STATIC`.
- All executables link `pico_stdlib` and only the libs they need.
- Stdio defaults (for picoprobe):
  ```cmake
  pico_enable_stdio_usb(<target> 0)
  pico_enable_stdio_uart(<target> 1)
  ```
- Add UF2 outputs:
  ```cmake
  pico_add_extra_outputs(<target>)
  ```