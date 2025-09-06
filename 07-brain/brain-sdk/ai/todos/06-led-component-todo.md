# Todo List: Brain-UI LED Component

## Setup
- [ ] Create a C++ class `Led`
- [ ] Add constructor with parameter for **GPIO pin number**
- [ ] Initialize GPIO pin for PWM output
- [ ] Store current LED state (on, off, blinking, brightness value, etc.)

## Basic Controls
- [ ] Implement `on()` method
  - Turn LED fully on (max brightness)
- [ ] Implement `off()` method
  - Turn LED fully off

## Brightness Control
- [ ] Implement `setBrightness(uint8_t value)` method
  - Value range: 0–255 (maps to PWM duty cycle)
  - Update PWM duty cycle accordingly

## Blinking
- [ ] Implement `blink(uint times, uint interval_ms)` method
  - Blink LED `times` with given `interval_ms`
  - Runs once, then returns to off state

- [ ] Implement **constant blinking**
  - Add `startBlink(uint interval_ms)` method
    - Blink indefinitely at given interval
  - Add `stopBlink()` method
    - Stop blinking and return to off state

## Update Handling
- [ ] Add `update()` method to be called in main loop
  - Handles timing for blink and constant blink
  - Keeps track of internal timers

## Callbacks
- [ ] Add callback when LED state changes (on/off)
- [ ] Add callback when LED blinking ends

## Testing
- [ ] Test single LED on one GPIO pin
- [ ] Test brightness adjustment (dim to bright)
- [ ] Test finite blink (e.g., 3 times at 200ms)
- [ ] Test constant blink start/stop
- [ ] Test edge cases (e.g., brightness=0, blink=0 times)