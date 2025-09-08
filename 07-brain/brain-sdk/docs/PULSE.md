# PULSE Component

## Overview
The PULSE component provides digital pulse input and output handling for the Hog Moduleur Brain module. It supports edge detection, debouncing, and event callbacks for pulse events.

## Features
- Handles pulse input (e.g., clock, trigger) via GPIO
- Supports pulse output (e.g., gate, trigger)
- Edge detection (rising/falling)
- Debounce logic for input pulses
- Event-driven: callbacks for pulse events

## Usage
1. **Initialization**: Create a `Pulse` instance, specifying the GPIO pin and direction (input/output).
2. **Polling**: Call `update()` in the main loop for input edge detection.
3. **Control**: For output, use methods to set or trigger pulses.
4. **Callbacks**: Register callbacks for pulse events (input only).

## Example
```cpp
#include "brain-io/pulse.h"

brain::io::Pulse clockIn(6, true); // GPIO 6, input
clockIn.setRisingCallback([](){
    // Clock pulse detected
});

brain::io::Pulse gateOut(7, false); // GPIO 7, output
gateOut.trigger(); // Send pulse

while (true) {
    clockIn.update();
}
```

## Notes
- Designed for Eurorack pulse signals (5V logic)
- Debounce logic is optimized for fast clocks
- Avoid blocking operations in callbacks
