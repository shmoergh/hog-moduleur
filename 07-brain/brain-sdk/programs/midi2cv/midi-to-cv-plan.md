# Plan: MIDI → CV (1 V/oct) with note stack & gate

## Goals
- Convert incoming MIDI **Note On/Off** to **control voltage** on **DAC channel A** (first DAC channel).
- Use **1 V/oct** mapping with **C0 = 0 V, C1 = 1 V, C2 = 2 V, …**
  → Voltage formula (for MIDI note `n`):
  `V = (n - 12) / 12`  (because MIDI C0 = 12)
- Drive **Gate** on the **PULSE** GPIO: **HIGH** on any active note(s), **LOW** when empty.
- Maintain a **last-note priority** buffer (stack) so releasing the top note falls back to the previous one.

## Architecture overview
- The program exists in folder `programs/midi2cv/` with `CMakeLists.txt` integrating Pico SDK + Brain SDK.
- **UART IRQ → MIDI parser**: read bytes, parse channel voice messages (Note On/Off), ignore others except optionally Sustain (CC64) later.
- **Note stack** (order-preserving set/stack):
  - On Note On (velocity>0): move or push note to top.
  - On Note Off (or Note On with velocity=0): remove note from stack.
  - Current note = stack.top() or “none”.
- **CV engine**:
  - Map `current_note` → volts via 1 V/oct formula.
  - Apply **calibration**: `Vout = offset + scale * V`. (per-unit trim)
  - Convert volts → **MCP4822** code and write via **SPI**.
- **Gate engine**:
  - If stack not empty → **PULSE = HIGH**; else **LOW**.
  - Optional: *retrigger mode* (later): brief LOW→HIGH pulse on repeated Note On of a different note.
- **Config**:
  - MIDI channel (0–15 or Omni).
  - DAC gain / full-scale and **scale/offset** trims (EEPROM/Flash or compile-time constants).
  - Optional: pitch bend range & sustain pedal handling (future).

## Data & math details
- **Note → Volt** (1 V/oct, C0=0 V):
  `Vraw = (note - 12) / 12.0`
  Examples: C0(12)=0 V, C1(24)=1 V, C2(36)=2 V.
- **Calibration**:
  `Vout = Voffset + Vscale * Vraw`
  Start with `Voffset=0`, `Vscale=1`. Adjust to match your analog chain.
- **Volt → DAC code** (MCP4822 12-bit):
  If full-scale is 4.096 V (gain×2), code ≈ `round( Vout / 4.096 * 4095 )`.
  Keep this in a small helper and centralize gain mode (×1/×2).

---

# TODO list (for the coding agent)

## 0) Project scaffolding
- [ ] Add dependency to `lib/brain-common` (GPIO, pins, SPI, timing helpers).
- [ ] Define file layout:
  - `src/main.cpp`
  - `src/midi_uart.h/.cpp` (IRQ RX & ringbuffer)
  - `src/midi_parser.h/.cpp` (state machine)
  - `src/note_stack.h/.cpp`
  - `src/dac_mcp4822.h/.cpp`
  - `src/gate.h/.cpp`
  - `src/config.h`
  - `src/calibration.h/.cpp` (optional NVM later)

## 1) Wire up hardware (use Brain SDK GPIO/SPIs)
- [ ] Include Brain common headers for pin IDs (MCP4822 SPI, chip-select, LDAC if used, PULSE pin).
- [ ] Initialize **SPI** for MCP4822 per Brain defaults.
- [ ] Initialize **DAC A** to a known state (write 0 V).
- [ ] Initialize **PULSE** pin as digital output, write **LOW**.
- [ ] Initialize **UART** for MIDI @ 31 250 baud, 8N1, enable RX IRQ.

## 2) MIDI UART RX ringbuffer
- [ ] Implement an **IRQ handler** that pushes incoming bytes into a lock-free ringbuffer (e.g., 256 bytes).
- [ ] Provide `bool midi_uart_pop(uint8_t* b)` for the main loop to consume bytes without blocking.

## 3) MIDI parser (channel voice only)
- [ ] Implement running-status aware parser for:
  - **Note On** (0x90–0x9F): note, velocity
  - **Note Off** (0x80–0x8F)
  - Treat **Note On with velocity=0** as Note Off
- [ ] Filter by configured channel (or Omni).
- [ ] Emit typed events: `MidiNoteOn{note,velocity}`, `MidiNoteOff{note}`.

## 4) Note stack (last-note priority)
- [ ] Implement `NoteStack` as **order-preserving container** (e.g., small `std::vector<uint8_t>` with linear ops; max 16–32 notes).
- [ ] Methods:
  - `pushTop(uint8_t note)` → if exists, **move to top**; else **append**.
  - `remove(uint8_t note)` → erase first occurrence.
  - `topOrNone()` → `std::optional<uint8_t>`.
  - `isEmpty()`.
- [ ] Unit-test logic locally (desktop build):
  Sequence from your example → verify fallback behavior.

## 5) CV mapping & DAC driver
- [ ] Implement `float note_to_vraw(uint8_t note)` = `(note - 12) / 12.0f`.
- [ ] Implement calibration: `Vout = offset + scale * Vraw` from `config.h`.
- [ ] Implement MCP4822 write:
  - Pack 12-bit value + control bits (channel A, gain, shutdown=active).
  - SPI transfer on the Brain’s MCP4822 pins.
  - Optional LDAC toggle if hardware requires.
- [ ] Implement `set_cv_for_note(optional<uint8_t> note)`:
  - If `note` present: compute code and write DAC A.
  - If not present: write **0 V** (code=0).

## 6) Gate engine
- [ ] Implement `Gate` with `begin()`, `setHigh()`, `setLow()` using Brain PULSE pin.
- [ ] Policy:
  - After **any Note On** → `Gate=HIGH`.
  - When stack becomes empty (after Note Off) → `Gate=LOW`.
- [ ] Optional (later): retrigger mode toggle—LOW for ~2 ms then HIGH on note change.

## 7) Main loop / event pump
- [ ] Loop:
  - Drain UART bytes → feed parser → dispatch events.
  - On **Note On**:
    - `noteStack.pushTop(note)`
    - `Gate.setHigh()`
    - `set_cv_for_note(noteStack.top())`
  - On **Note Off**:
    - `noteStack.remove(note)`
    - If empty:
      - `Gate.setLow()`
      - `set_cv_for_note(None)` → 0 V
    - Else:
      - `set_cv_for_note(noteStack.top())`
- [ ] Keep loop non-blocking; no delays except brief optional retrigger pulse.

## 8) Configuration & calibration
- [ ] `config.h` constants:
  - `MIDI_CHANNEL` (0–15) or `OMNI=true`
  - `DAC_GAIN_X2` (bool), `DAC_FS_VOLTS` (e.g., 4.096)
  - `CAL_OFFSET` (float, volts), `CAL_SCALE` (float)
- [ ] Provide a simple serial (USB CDC) command stub (optional) to print or tweak calibration at runtime; persist later.

## 9) Edge cases & robustness
- [ ] Handle **Note On** for the same note repeatedly (move to top; no duplicates).
- [ ] Handle **velocity=0** as Note Off.
- [ ] Ignore real-time MIDI (0xF8 etc.) anywhere in stream.
- [ ] Parser resilience on **running status** and interleaved real-time bytes.
- [ ] On watchdog or reboot, ensure **Gate=LOW** and **DAC=0 V** at startup.
- [ ] Optional future: **Sustain pedal (CC64)** latch behavior; **Pitch Bend** (adjust Vraw continuously); **Transpose**; **Note priority modes** (last/high/low).

## 10) Build & deploy
- [ ] Add CMake targets and include paths for `lib/brain-common`.
- [ ] Compile for Pico; flash UF2 as per Brain’s standard flow.
- [ ] Bench test:
  - Send MIDI: C0 (12) → 0.000 V ± error
  - C1 (24) → 1.000 V; C2 (36) → 2.000 V, etc.
  - Verify Gate HIGH while stack non-empty; LOW when empty.
- [ ] Calibrate `CAL_SCALE`/`CAL_OFFSET` until 1 V/oct tracks across several octaves.

---

## Pseudocode sketch (agent reference)

```cpp
int main() {
  brain::init_clocks();
  Dac4822 dac(spi0, pins::MCP4822_CS, /*gainX2=*/true);
  Gate gate(pins::PULSE_OUT);
  MidiUart midi(uart1, pins::MIDI_RX);
  MidiParser parser(config::MIDI_CHANNEL);
  NoteStack stack;

  dac.writeVoltsA(0.0f);
  gate.low();

  for (;;) {
    uint8_t b;
    while (midi.pop(&b)) {
      if (auto ev = parser.feed(b)) {
        std::visit([&](auto& e){
          using T = std::decay_t<decltype(e)>;
          if constexpr (std::is_same_v<T, MidiNoteOn>) {
            stack.pushTop(e.note);
            gate.high();
            set_cv_for_note(dac, stack.top());
          } else if constexpr (std::is_same_v<T, MidiNoteOff>) {
            stack.remove(e.note);
            if (stack.isEmpty()) {
              gate.low();
              set_cv_for_note(dac, std::nullopt);
            } else {
              set_cv_for_note(dac, stack.top());
            }
          }
        }, *ev);
      }
    }
    tight_loop_contents(); // Pico SDK idle hint
  }
}
```

Helper:
```cpp
static inline void set_cv_for_note(Dac4822& dac, std::optional<uint8_t> note) {
  float v = 0.0f;
  if (note) {
    float vraw = (int(*note) - 12) / 12.0f;
    v = config::CAL_OFFSET + config::CAL_SCALE * vraw;
  }
  dac.writeVoltsA(v);
}
```

---

## Notes & assumptions
- **Pins & buses**: use the exact SPI/UART/PULSE symbols from `lib/brain-common` (do not hardcode).
- **DAC scaling** depends on analog chain; we expose `CAL_OFFSET` + `CAL_SCALE` to hit exact 1 V/oct at the Eurorack output.
- **First DAC channel** = MCP4822 **Channel A**.
- **Gate** = Brain **PULSE** output GPIO. Keep it a simple digital high/low (no PWM).
