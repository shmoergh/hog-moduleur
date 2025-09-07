# TODO — Brain-IO `pulse` (digital in/out with inversion) ✅ COMPLETED

> Goal: Provide a tiny, predictable API to read a transistor-inverted digital **input** and drive a transistor-inverted digital **output**. SDK handles inversion transparently. Pulse **length is NOT part of the SDK** (users will set/clear the output themselves).

---

## 0) Define scope & constraints
- [x] Single module/class: `Pulse` (or `brain_io::Pulse`), owning one input pin and one output pin.
- [x] Input logic is **inverted** by hardware → SDK returns **true** when the physical jack is logically ON.
- [x] Output stage is **active-low** (hardware idles HIGH) → SDK's `set(true)` pulls line **active**, `set(false)` releases to idle.
- [x] No pulse timing, envelopes, or PWM here. Just on/off and edge callbacks (optional).
- [x] Must be safe on boot: **do not** glitch the output low during init.

---

## 1) Public API (header first)
- [x] `Pulse(uint in_gpio, uint out_gpio)` constructor.
- [x] `bool Read()` — returns **logical** value (inversion handled).
- [x] `bool ReadRaw()` — returns raw GPIO level (for debugging).
- [x] `void Set(bool on)` — **logical** set; `on=true` asserts (drives output active), `on=false` de-asserts (idle).
- [x] `bool Get()` — returns last commanded **logical** output state (cached).
- [x] `void Begin()` — configures GPIO directions, pull-ups/downs, and ensures safe idle.
- [x] `void End()` — optional: return pins to inputs/high-impedance (for hot-swap/unload).
- [x] Callbacks:
  - [x] `void OnRise(std::function<void()> cb)` — logical low→high.
  - [x] `void OnFall(std::function<void()> cb)` — logical high→low.
  - [x] `void Poll()` — call in main loop to detect edges if IRQs aren't used.
- [x] Optional filtering:
  - [x] `void SetInputGlitchFilterUs(uint32_t us)` — 0 = off; cheap de-glitch in software.

---

## 2) GPIO setup & electrical expectations
- [x] Configure `in_gpio` as input; set it to **pull-up enabled**
- [x] Configure `out_gpio` as output and **drive it to idle (HIGH)** before switching the pin to output to avoid a low pulse glitch.
- [x] Document that the output stage is active-low due to transistor; SDK already inverts it for the user.

---

## 3) Inversion logic
- [x] **Input:** `Read()` returns `!gpio_get(in_gpio)`; `ReadRaw()` returns `gpio_get(in_gpio)`.
- [x] **Output:** `Set(true)` → drive **LOW**; `Set(false)` → drive **HIGH**. Cache logical state for `Get()`.

---

## 4) Edge detection (polling implementation)
- [x] Store previous **logical** input state.
- [x] In `Poll()`, read current logical value, compare to previous:
  - [x] If `false→true`: invoke `OnRise` if set.
  - [x] If `true→false`: invoke `OnFall` if set.

---

## 5) Interrupt-driven edges
- [x] Provide `EnableInterrupts()` that installs a GPIO IRQ on **raw** input pin for both edges.
- [x] In ISR: timestamp and enqueue raw edges; in main loop `Poll()` drains queue, applies inversion, then fires callbacks.
- [x] Provide `DisableInterrupts()`.

---

## 6) Safety & correctness
- [x] Ensure **no output low blip** during `Begin()`: write HIGH to SIO OUT register before setting pin function to output.
- [x] Make API idempotent: calling `Set()` with the current logical state shouldn't toggle the line.
- [x] Thread-/core-safety note: if used across cores/IRQs, guard shared state with minimal critical sections.

---

## 7) Build & integration
- [x] Add `pulse.[h|cpp]` to `brain-io` library (use `brain-io/include/brain-io` for header files), expose in `CMakeLists.txt`.
- [x] Doxygen (or brief header docs) for every public method.

---

## Implementation Summary

✅ **Completed**: The Brain-IO Pulse component has been successfully implemented with the following features:

### Core Files Created:
- `lib/brain-io/include/brain-io/pulse.h` - Header with complete API
- `lib/brain-io/pulse.cpp` - Implementation with all features
- `programs/hardware-test/pulse_test.cpp` - Test program demonstrating functionality

### Features Implemented:
1. **Safe GPIO Initialization**: Output pin set to idle (HIGH) before direction change to prevent glitches
2. **Hardware Inversion Handling**: Transparent inversion for both input and output signals
3. **Edge Detection**: Both polling and interrupt-driven edge detection with callback support
4. **Glitch Filtering**: Software-based input debouncing with configurable duration
5. **Idempotent Operations**: Set operations only change GPIO when state actually changes
6. **Comprehensive Documentation**: Doxygen-compatible comments for all public methods

### Integration:
- Added to `brain-io` library CMakeLists.txt
- Included in main brain-io.h header (C++ only)
- Test integration added to hardware-test program
- Successfully builds with the existing SDK

The implementation fully satisfies all requirements from the TODO specification and provides a robust, predictable API for digital I/O with hardware inversion support.
