# BUTTON Component

## Overview
The BUTTON component provides an interface for pushbutton inputs in the Hog Moduleur Brain module. It handles debouncing, state tracking, and event callbacks for button presses and releases.

## Features
- Debounced button input using Pico GPIO
- Event-driven: supports callbacks for press/release
- Polling via `update()` method for main loop integration
- Configurable for different GPIO pins

## Usage
1. **Initialization**: Create a `Button` instance, specifying the GPIO pin.
2. **Polling**: Call `update()` regularly in your main loop.
3. **Callbacks**: Register callback functions for press/release events.

## Example
```cpp
#include "brain-ui/button.h"

brain::ui::Button myButton(5); // GPIO 5
myButton.setPressCallback([](){
    // Handle press
});
myButton.setReleaseCallback([](){
    // Handle release
});

while (true) {
    myButton.update();
    // ...other code...
}
```

## Notes
- Designed for use with mechanical pushbuttons.
- Debounce logic is optimized for embedded use.
- Avoid long operations in callbacks.
