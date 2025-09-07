# TODO — Brain-IO `pulse` (digital in/out with inversion)

> Goal: Provide a tiny, predictable API to read a transistor-inverted digital **input** and drive a transistor-inverted digital **output**. SDK handles inversion transparently. Pulse **length is NOT part of the SDK** (users will set/clear the output themselves).

---

## 0) Define scope & constraints
- [ ] Single module/class: `Pulse` (or `brain_io::Pulse`), owning one input pin and one output pin.
- [ ] Input logic is **inverted** by hardware → SDK returns **true** when the physical jack is logically ON.
- [ ] Output stage is **active-low** (hardware idles HIGH) → SDK’s `set(true)` pulls line **active**, `set(false)` releases to idle.
- [ ] No pulse timing, envelopes, or PWM here. Just on/off and edge callbacks (optional).
- [ ] Must be safe on boot: **do not** glitch the output low during init.

---

## 1) Public API (header first)
- [ ] `Pulse(uint in_gpio, uint out_gpio)` constructor.
- [ ] `bool read()` — returns **logical** value (inversion handled).
- [ ] `bool readRaw()` — returns raw GPIO level (for debugging).
- [ ] `void set(bool on)` — **logical** set; `on=true` asserts (drives output active), `on=false` de-asserts (idle).
- [ ] `bool get()` — returns last commanded **logical** output state (cached).
- [ ] `void begin()` — configures GPIO directions, pull-ups/downs, and ensures safe idle.
- [ ] `void end()` — optional: return pins to inputs/high-impedance (for hot-swap/unload).
- [ ] Callbacks:
  - [ ] `void onRise(std::function<void()> cb)` — logical low→high.
  - [ ] `void onFall(std::function<void()> cb)` — logical high→low.
  - [ ] `void poll()` — call in main loop to detect edges if IRQs aren’t used.
- [ ] Optional filtering:
  - [ ] `void setInputGlitchFilterUs(uint32_t us)` — 0 = off; cheap de-glitch in software.

---

## 2) GPIO setup & electrical expectations
- [ ] Configure `in_gpio` as input; set it to **pull-up enabled**
- [ ] Configure `out_gpio` as output and **drive it to idle (HIGH)** before switching the pin to output to avoid a low pulse glitch.
- [ ] Document that the output stage is active-low due to transistor; SDK already inverts it for the user.

---

## 3) Inversion logic
- [ ] **Input:** `read()` returns `!gpio_get(in_gpio)`; `readRaw()` returns `gpio_get(in_gpio)`.
- [ ] **Output:** `set(true)` → drive **LOW**; `set(false)` → drive **HIGH**. Cache logical state for `get()`.

---

## 4) Edge detection (polling implementation)
- [ ] Store previous **logical** input state.
- [ ] In `poll()`, read current logical value, compare to previous:
  - [ ] If `false→true`: invoke `onRise` if set.
  - [ ] If `true→false`: invoke `onFall` if set.

---

## 5) Interrupt-driven edges
- [ ] Provide `enableInterrupts()` that installs a GPIO IRQ on **raw** input pin for both edges.
- [ ] In ISR: timestamp and enqueue raw edges; in main loop `poll()` drains queue, applies inversion, then fires callbacks.
- [ ] Provide `disableInterrupts()`.

---

## 6) Safety & correctness
- [ ] Ensure **no output low blip** during `begin()`: write HIGH to SIO OUT register before setting pin function to output.
- [ ] Make API idempotent: calling `set()` with the current logical state shouldn’t toggle the line.
- [ ] Thread-/core-safety note: if used across cores/IRQs, guard shared state with minimal critical sections.

---

## 7) Build & integration
- [ ] Add `pulse.[h|cpp]` to `brain-io` library (use `brain-io/include/brain-io` for header files), expose in `CMakeLists.txt`.
- [ ] Doxygen (or brief header docs) for every public method.