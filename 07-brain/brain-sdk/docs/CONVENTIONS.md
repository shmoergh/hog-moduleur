# Brain SDK – C/C++ Conventions (Google Style, Embedded-friendly)

> Scope: All code in `brain-sdk/` (`lib/*`, `programs/*`).
> Baseline: Google C++ Style Guide (naming, formatting, includes) with embedded tweaks.

---

## 0) TL;DR Checklist
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

---

## 1) File Layout & Naming
- **Folders**: use kebab-case → `lib/brain-io/`, `lib/brain-dsp/`.
- **Files**: `lower_case_with_underscores.h` / `.cc` (or `.cpp`).
- **One module per pair**: `brain-io/gpio_driver.h` ↔ `brain-io/gpio_driver.cc`.
- **No mega-files**: prefer small, testable units.

**Header guard** (prefer `#pragma once` for brevity):
```cpp
#pragma once
// or, if guards required:
// #ifndef BRAIN_IO_GPIO_DRIVER_H_
// #define BRAIN_IO_GPIO_DRIVER_H_
// ...
// #endif  // BRAIN_IO_GPIO_DRIVER_H_
```

---

## 2) Includes & Dependencies
Order in both headers and sources:
1. C system headers (`<stdint.h>`, `<stddef.h>`)
2. C++ standard (`<array>`, `<span>`, `<optional>`)
3. Pico/3rd-party (`<pico/stdlib.h>`, `hardware/gpio.h`)
4. Project headers (`"brain-io/gpio_driver.h"`)

One include per line; no wildcard includes. Prefer forward declarations in headers where possible.

---

## 3) Formatting
- **Indent**: 4 spaces; **Line length**: 100.
- **Braces**: K&R style.
```cpp
if (ready) {
	DoThing();
} else {
  	HandleError();
}
```
- **Namespace**: wrap entire file; avoid anonymous namespaces in headers.
```cpp
namespace brain::io {
// ...
}  // namespace brain::io
```

---

## 4) Naming
- **Namespaces**: `lower_case` (e.g., `brain::io`).
- **Types / Classes / Structs / Enums**: `PascalCase` (e.g., `AdcSampler`).
- **Functions & Methods**: `PascalCase` (e.g., `Init()`, `ReadChannel()`).
- **Variables**: `lower_case_with_underscores` (`sample_rate_hz`).
  - **Data members**: `trailing_underscore_` (e.g., `gpio_`).
- **Constants**: `kPascalCase` (`kMaxChannels`).
- **Macros**: `ALL_CAPS_WITH_UNDERSCORES` (use sparingly).
- **Aliases**: `using Sample = uint16_t;` (avoid `typedef` in new code).

---

## 5) Language Use
### C++
- **C++17+** (or project default). Prefer `<array>`, `<span>`, `<optional>`, `<chrono>`.
- **No exceptions / no RTTI** by default (size & determinism). Use `Status` returns.
- Prefer **`enum class`**; avoid unscoped enums.
- Prefer **`constexpr`** & **`const`** over macros; **`static`** for internal linkage.
- RAII for HW resources when feasible (GPIO claim/release), but keep ISRs free of heavy constructs.

### C (when used)
- Follow same formatting; `snake_case` for functions/vars is acceptable in pure-C modules.
- Use fixed-width integers from `<stdint.h>`.

---

## 6) APIs & Headers
- Headers are **self-contained** (compile on their own).
- Forward-declare where practical; don’t `#include` what you don’t use.
- Public API gets a brief Doxygen block:
```cpp
/// Samples N channels via a 74HC4051 into the Pico ADC.
/// Blocking; ~200 µs per channel typical.
/// @param channels logical channel indices [0..2]
/// @return last sampled value in 12-bit right-aligned format.
uint16_t SampleOnce(int channel);
```

---

## 7) Error Handling & Logging
- Return `bool` / small `Status` enums for expected errors; no exceptions.
- For fatal HW config errors in bring-up/demo code, `assert()` is acceptable (not in library hot paths).
- Logging:
  - Default to **UART stdio** (`pico_enable_stdio_uart(... 1)`).
  - Use minimal `fprintf(stderr, ...)`/`printf()` in libraries; let programs decide verbosity.

---

## 8) Embedded Rules (Pico/RP2040)
- **ISRs**: `static inline` where sensible; **no malloc/new**, no `printf`, keep <50 µs.
- **Volatile** only for MMIO or truly shared flags; pair with memory barriers when needed.
- **Concurrency**: prefer RP2040 primitives (IRQ disable windows, multicore queues, spinlocks) with brief critical sections.
- **Timing**: use `absolute_time_t` / `busy_wait_us()` from pico-time; avoid ad-hoc loops.
- **GPIO**: configure once in init; document pin ownership in module comments.
- **DMA/PWM/ADC**: place registers/config in one module; avoid cross-module control.

---

## 9) Tests & Examples
- Unit tests for pure logic (host-buildable) go under `tests/` (future).
- Hardware demos live in `programs/<name>/`.
- Keep examples tiny: one concept per example.

---

## 10) CMake Conventions
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

---

## 11) Comments & Documentation
- Prefer short, clear comments explaining *why*, not *what*.
- File header (top 6 lines): one-sentence purpose, dependencies, hardware notes, author/owner.
- Use Doxygen for public APIs; keep private helpers undocumented unless non-obvious.

---

## 12) Sample Skeleton

```cpp
// adc-sampler.h
#pragma once
#include <cstdint>
namespace brain::io {

class AdcSampler {
 public:
  bool Init();                   // config ADC + mux GPIOs
  uint16_t SampleChannel(int channel);  // 0..2

 private:
  void SelectMux(int channel);
  uint16_t ReadAdcOnce();

  int adc_channel_ = 0;
};

}  // namespace brain::io
```

```cpp
// adc-sampler.cc
#include "brain-io/adc-sampler.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

namespace brain::io {

bool AdcSampler::Init() {
  adc_init();
  // configure GPIOs, etc.
  return true;
}

uint16_t AdcSampler::SampleChannel(int channel) {
  SelectMux(channel);
  sleep_us(10);     // settle
  (void)ReadAdcOnce(); // throw-away
  return ReadAdcOnce();
}

void AdcSampler::SelectMux(int channel) {
  // drive S0/S1, keep S2 low
}

uint16_t AdcSampler::ReadAdcOnce() {
  return adc_read();
}

}  // namespace brain::io
```

---

## 13) Things to Avoid
- Global mutable state (except tightly-scoped singletons for HW where necessary).
- Hidden work in constructors; prefer `Init()` returning `bool`.
- Long lambdas or templates in embedded hot paths (code size bloat).
- Overuse of macros; prefer `constexpr`, `inline`, and typed enums.

---
