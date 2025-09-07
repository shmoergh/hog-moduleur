# TODO — Multiplexed Pot Reading (CD74HC4051, 3 pots, 2 addr lines)

## 0) Assumptions
- Pico @ 3.3 V; CD74HC4051 powered from 3.3 V (logic compatible).
- 4051 pins used: `X/Z` (common) → one Pico ADC pin; `S0,S1` → Pico GPIOs; `EN` (active-low) → Pico GPIO (optional; tie low if not used).
- 3 pots wired to channels `Y0..Y2`; unused channels left floating or pulled appropriately.
- No external driver library; use SDK GPIO + ADC only.

## 1) Configuration & Pin Map
[x] Define a small config struct:
  - ADC pin (GPIO number) used for 4051 common.
  - GPIO pins for `S0`, `S1`, and optional `EN`.
  - Number of pots = 3; channel map array `[0,1,2]`.
  - ADC resolution output (e.g., 12-bit raw, optional scaled).
  - Settling delay after switch (µs).
  - Samples per reading (oversampling count).
  - Change threshold / hysteresis.
  - Optional per-pot calibration.
[x] Provide a single function to set this config at init.

## 2) GPIO & ADC Init
[x] Init ADC (enable HW, select ADC input GPIO).
[x] Init `S0`/`S1`/`EN` as outputs; drive `EN` low.
[x] Drive `S0`/`S1` low initially; small guard delay.

## 3) Address Line Bit-Banging
[x] Implement `set_mux_channel(uint8_t ch)`:
  - Mask to 2 bits, write to `S0` and `S1`.
  - Optional bounds check.
[x] Optional: `mux_enable()` / `mux_disable()`.

## 4) Read Path & Timing
[x] Implement `read_channel_once(uint8_t ch)`:
  - Set address lines.
  - Busy-wait for settling (~5–10 µs).
  - Take N ADC samples, sum and average.
  - Return raw averaged sample.
[x] Implement `read_pot(index)`:
  - Map `index` → physical channel.
  - Call `read_channel_once()`.
  - Apply calibration and smoothing.
  - Return final value.

## 5) Scanner (Round-Robin)
[x] Implement `scan_pots()`:
  - Loop indices 0..2, call `read_pot()`.
  - If value changed beyond threshold, update state.
  - Optional callback: `onChange(index, value)`.
[x] Provide usage modes:
  - Polling in main loop.
  - Timer/IRQ every X ms.

## 6) API Surface
[x] `void pots_init(const PotsConfig* cfg);`
[x] `uint16_t pot_read_raw(uint8_t index);`
[x] `uint16_t pot_get(uint8_t index);`
[x] `void pots_scan(void);`
[x] `void pots_set_on_change(void (*cb)(uint8_t idx, uint16_t val));`
[x] `void pots_set_calibration(uint8_t idx, uint16_t min, uint16_t max);`

## 7) Noise, Stability, Performance
[x] Default settling delay: ~8 µs.
[x] Samples per read: 4–8.
[x] Hysteresis: ~4–8 ADC counts.
[x] Optional 1-pole IIR filter.
[x] Guard against rail hits.

## 8) Calibration Utilities
[x] Calibration mode: sweep pot, record min/max.
[x] Apply linear scaling `(raw - min) * 4095 / (max - min)`.
[x] Stub persistent storage hooks.

## 9) Error Handling
[x] Validate GPIOs/ADC pin combo.
[x] Handle `EN` high (disabled).
[x] Protect against invalid args.

## 10) Tests / Bring-Up
[x] Simple hardware test in `hardware-test/main.cpp` program.