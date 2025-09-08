# LED Component

## Overview
The LED component manages individual LEDs in the Hog Moduleur Brain module, supporting brightness control via PWM and event-driven blinking.

## Features
- Controls LED state (on/off)
- Adjustable brightness using Pico PWM
- Supports blinking with configurable duration
- Event callback for blink completion

## Usage
1. **Initialization**: Create an `Led` instance, specifying the GPIO pin and PWM channel.
2. **Control**: Use methods to set brightness, turn on/off, or start a blink.
3. **Polling**: Call `update()` in the main loop for blink timing.
4. **Callbacks**: Register a callback for blink completion if needed.

## Example
```cpp
#include "brain-ui/led.h"

brain::ui::Led statusLed(10, 2); // GPIO 10, PWM slice 2
statusLed.setBrightness(128); // 50% brightness
statusLed.blink(500); // Blink for 500 ms
statusLed.setBlinkCallback([](){
    // Blink finished
});

while (true) {
    statusLed.update();
}
```

## Notes
- Designed for transistor-driven LEDs (Eurorack compatible)
- Avoid blocking operations in callbacks
- Multiple LEDs can be managed independently
