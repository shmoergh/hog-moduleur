# POT_MULTIPLEXER Component

## Overview
The POT_MULTIPLEXER component enables reading multiple potentiometers using a single Pico ADC channel, via a multiplexer (e.g., 74HC4051). It manages channel selection, settling time, and value retrieval.

## Features
- Supports multiple pots on one ADC input
- Handles multiplexer channel switching
- Manages settling and sampling timing
- Provides per-pot value access
- Optional change callbacks for each pot

## Usage
1. **Initialization**: Create a `PotMultiplexer` instance, specifying ADC channel and GPIOs for mux control.
2. **Polling**: Call `update()` in the main loop to sample all pots.
3. **Access**: Use `getValue(channel)` to read a pot's value.
4. **Callbacks**: Register change callbacks for individual pots if needed.

## Example
```cpp
#include "brain-ui/pot_multiplexer.h"

brain::ui::PotMultiplexer pots(0, {2, 3, 4}); // ADC0, S0/S1/S2 GPIOs
pots.setChangeCallback(1, [](uint16_t value){
    // Pot 1 changed
});

while (true) {
    pots.update();
    uint16_t pot0 = pots.getValue(0);
}
```

## Notes
- Designed for Eurorack pots (10k-100k typical)
- Settling time is managed automatically
- Avoid long operations in callbacks
