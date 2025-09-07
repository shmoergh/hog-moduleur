---
applyTo: '**/*.h,**/*.cpp,**/*.c'
---
# Project C/C++ Coding Guidelines (Google Style, Embedded-friendly)

> Baseline: Google C++ Style Guide (naming, formatting, includes) with embedded tweaks.

---

Apply the [general coding guidelines](../instructions/general-coding-guidelines.instructions.md) to all code. When found conflicting information, use the instructions in this file.

---

## File Layout & Naming
- **Folders**: use kebab-case → `lib/brain-io/`, `lib/brain-dsp/`.
- **Files**: `lower_case_with_underscores.h` / `.cc` (or `.cpp`).
- **One module per pair**: `brain-io/gpio_driver.h` ↔ `brain-io/gpio_driver.cc`.
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

## Formatting
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

## Naming
- **Namespaces**: `lower_case` (e.g., `brain::io`).
- **Types / Classes / Structs / Enums**: `PascalCase` (e.g., `AdcSampler`).
- **Functions & Methods**: `camelCase` (e.g., `init()`, `readChannel()`).
- **Variables**: `lower_case_with_underscores` (`sample_rate_hz`).
  - **Data members**: `trailing_underscore_` (e.g., `gpio_`).
- **Constants**: `kPascalCase` (`kMaxChannels`).
- **Macros**: `ALL_CAPS_WITH_UNDERSCORES` (use sparingly).
- **Aliases**: `using Sample = uint16_t;` (avoid `typedef` in new code).

---

## Language Use
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

---

## Comments & Documentation
- Prefer short, clear comments explaining *why*, not *what*.
- File header (top 6 lines): one-sentence purpose, dependencies, hardware notes, author/owner.
- Use Doxygen for public APIs; keep private helpers undocumented unless non-obvious.

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
// adc-sampler.cc
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
---
applyTo: '**/*.h,**/*.cpp,**/*.c'
---
# Project C/C++ Coding Guidelines (Google Style, Embedded-friendly)

> Baseline: Google C++ Style Guide (naming, formatting, includes) with embedded tweaks.

---

Apply the [general coding guidelines](../instructions/general-coding-guidelines.instructions.md) to all code. When found conflicting information, use the instructions in this file.

---

## File Layout & Naming
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

## Includes & Dependencies
Order in both headers and sources:
1. C system headers (`<stdint.h>`, `<stddef.h>`)
2. C++ standard (`<array>`, `<span>`, `<optional>`)
3. Pico/3rd-party (`<pico/stdlib.h>`, `hardware/gpio.h`)
4. Project headers (`"brain-io/gpio_driver.h"`)

One include per line; no wildcard includes. Prefer forward declarations in headers where possible.

---

## Formatting
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

## Naming
- **Namespaces**: `lower_case` (e.g., `brain::io`).
- **Types / Classes / Structs / Enums**: `PascalCase` (e.g., `AdcSampler`).
- **Functions & Methods**: `camelCase` (e.g., `init()`, `readChannel()`).
- **Variables**: `lower_case_with_underscores` (`sample_rate_hz`).
  - **Data members**: `trailing_underscore_` (e.g., `gpio_`).
- **Constants**: `kPascalCase` (`kMaxChannels`).
- **Macros**: `ALL_CAPS_WITH_UNDERSCORES` (use sparingly).
- **Aliases**: `using Sample = uint16_t;` (avoid `typedef` in new code).

---

## Language Use
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

---

## Comments & Documentation
- Prefer short, clear comments explaining *why*, not *what*.
- File header (top 6 lines): one-sentence purpose, dependencies, hardware notes, author/owner.
- Use Doxygen for public APIs; keep private helpers undocumented unless non-obvious.

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
// adc-sampler.cc
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
