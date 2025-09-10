# Repository Guidelines

## Project Structure & Modules
- `lib/` reusable libraries: `brain-io`, `brain-ui`, `brain-common` (public headers under `lib/<mod>/include/<mod>/`). Example: `lib/brain-ui/include/brain-ui/button.h`.
- `programs/` firmware apps (one folder per app). Example: `programs/hardware-test/` builds `hardware-test`.
- `pico-sdk/` Pico SDK (git submodule). `build/` CMake output. Docs live in `docs/`.

---

## Build, Flash, and Dev Commands
- Configure and build:
  - `cmake -B build -G "Unix Makefiles"` (add flags like `-DPICO_BOARD=pico2` if needed)
  - `cmake --build build -j`
- Artifacts: `build/programs/<name>/<name>.{elf,uf2}`. Flash by holding BOOTSEL, mount `RPI-RP2`, then copy the `.uf2`.
- Scaffold a new app: `./scripts/new-program.sh <name>` then reconfigure/rebuild.

---

## File Layout & Naming
- **Folders**: use kebab-case → `lib/brain-io/`, `lib/brain-dsp/`.
- **Files**: use kebab-case → `brain-io.h` / `.cpp`
- **One module per pair**: `brain-io/gpio-driver.h` ↔ `brain-io/gpio-driver.cpp`.
- **No mega-files**: prefer small, testable units.

**Header guard** (prefer `#ifndef` for brevity):
```cpp
#ifndef BRAIN_IO_GPIO_DRIVER_H
#define BRAIN_IO_GPIO_DRIVER_H
...
#endif  // BRAIN_IO_GPIO_DRIVER_H_
```

---

## Includes & Dependencies
Order in both headers and sources:
1. C system headers (`<stdint.h>`, `<stddef.h>`)
2. C++ standard (`<array>`, `<span>`, `<optional>`)
3. Pico/3rd-party (`<pico/stdlib.h>`, `hardware/gpio.h`)
4. Project headers (`"brain-io/gpio_driver.h"`)

One include per line; no wildcard includes. Prefer forward declarations in headers where possible.

---

## Coding Style & Naming
- C/C++ formatted by `.clang-format` (Google base):
  - Tabs only (width 4), column limit 100, sorted/regrouped includes, left-aligned pointers.
  - Format: `clang-format -style=file -i path/to/file.cpp`.
- **Braces**: K&R style.
```cpp
if (ready) {
    doThing();
} else {
    handleError();
}
```
- **Namespace**: wrap entire file; avoid anonymous namespaces in headers.
```cpp
namespace brain::io {
// ...
}  // namespace brain::io
```
- File layout: keep public headers in `include/<module>/…`, implementation in `.cpp`. Keep target name = folder name (e.g., `programs/foo/foo.cpp` → `foo`).
- **Namespaces**: `lower_case` (e.g., `brain::io`).
- **Types / Classes / Structs / Enums**: `PascalCase` (e.g., `AdcSampler`).
- **Functions & Methods**: `camelCase` (e.g., `init()`, `readChannel()`).
- **Variables**: `lower_case_with_underscores` (`sample_rate_hz`).
  - **Data members**: `trailing_underscore_` (e.g., `gpio_`).
- **Constants**: `kPascalCase` (`kMaxChannels`).
- **Macros**: `ALL_CAPS_WITH_UNDERSCORES` (use sparingly).
- **Aliases**: `using Sample = uint16_t;` (avoid `typedef` in new code).

---

## Error Handling & Logging
- Return `bool` / small `Status` enums for expected errors; no exceptions.
- For fatal HW config errors in bring-up/demo code, `assert()` is acceptable (not in library hot paths).
- Logging:
  - Default to **UART stdio** (`pico_enable_stdio_uart(... 1)`).
  - Use minimal `fprintf(stderr, ...)`/`printf()` in libraries; let programs decide verbosity.

---

## Embedded Rules (Pico/RP2040)
- **ISRs**: `static inline` where sensible; **no malloc/new**, no `printf`, keep <50 µs.
- **Volatile** only for MMIO or truly shared flags; pair with memory barriers when needed.
- **Concurrency**: prefer RP2040 primitives (IRQ disable windows, multicore queues, spinlocks) with brief critical sections.
- **Timing**: use `absolute_time_t` / `busy_wait_us()` from pico-time; avoid ad-hoc loops.
- **GPIO**: configure once in init; document pin ownership in module comments.
- **DMA/PWM/ADC**: place registers/config in one module; avoid cross-module control.
- Only use floats when absolute necessary

---

## Comments & Documentation
- Prefer short, clear comments explaining *why*, not *what*.
- File header (top 6 lines): one-sentence purpose, dependencies, hardware notes, author/owner.
- Use Doxygen for public APIs; keep private helpers undocumented unless non-obvious.
- Multiline Doxygen comments should use /** */ style

---

## Sample Skeleton

```cpp
// adc-sampler.h
#pragma once
#include <cstdint>
namespace brain::io {

class AdcSampler {
  public:
    bool init();                   // config ADC + mux GPIOs
    uint16_t sampleChannel(int channel);  // 0..2

  private:
    void selectMux(int channel);
    uint16_t readAdcOnce();

  int adc_channel_ = 0;
};

}  // namespace brain::io
```

```cpp
// adc-sampler.cpp
#include "brain-io/adc-sampler.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

namespace brain::io {

bool AdcSampler::init() {
  adc_init();
  // configure GPIOs, etc.
  return true;
}

uint16_t AdcSampler::sampleChannel(int channel) {
  selectMux(channel);
  sleep_us(10);     // settle
  (void)readAdcOnce(); // throw-away
  return readAdcOnce();
}

void AdcSampler::selectMux(int channel) {
  // drive S0/S1, keep S2 low
}

uint16_t AdcSampler::readAdcOnce() {
  return adc_read();
}

}  // namespace brain::io
```

---

## Things to Avoid
- Global mutable state (except tightly-scoped singletons for HW where necessary).
- Hidden work in constructors; prefer `init()` returning `bool`.
- Long lambdas or templates in embedded hot paths (code size bloat).
- Overuse of macros; prefer `constexpr`, `inline`, and typed enums.

---

## Testing Guidelines
- No formal unit test framework yet. Use `programs/hardware-test/` or create small program-level test apps under `programs/<feature>/` (e.g., `*_test.cpp`) to validate modules on hardware.
- Prefer deterministic, minimal examples. Document expected behavior in `docs/` when adding new modules.

---

## Commit & Pull Request Guidelines
- Commits: short, imperative, and scoped. Examples:
  - "Add UART processing in MIDI class"
  - "Update pulse to use global GPIO settings"
- PRs: include a clear description, affected modules, build output path(s), and any hardware steps to verify. Link related docs updates/screenshots or serial logs when relevant. One feature/fix per PR where possible.

---

## Security & Configuration Tips
- Ensure the Pico SDK submodule is initialized: `git submodule update --init --recursive`.
- Toolchain requirements and recommended VSCode settings are in `docs/SETUP.md`. Use OpenOCD/Picoprobe for debugging; set `executable` to the desired `.elf` in `.vscode/launch.json`.

