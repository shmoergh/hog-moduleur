# Todo list: Pot component for brain-ui

## Class setup
- [ ] Create a `Pot` C++ class in the `brain-ui` library
- [ ] Constructor should accept:
  - [ ] `gpio_pin` (the ADC input pin number)
  - [ ] `resolution_bits` (number of bits from 2 to 12)

## Internal state
- [ ] Store current raw ADC reading
- [ ] Store scaled value according to `resolution_bits`
- [ ] Store last value for change detection
- [ ] Store callback function pointer for `onChange`

## Initialization
- [ ] Set up the ADC pin for input in the constructor
- [ ] Configure ADC hardware (init, select input channel)

## Functionality
- [ ] Implement `setOnChangeCallback(callback_function)` method
  - Store a function pointer or `std::function` that is called when the value changes
- [ ] Implement `update()` method
  - Read raw ADC value
  - Scale it down to the selected resolution (`2^n`)
  - Compare with previous value
  - If value changed, update state and call `onChange` callback with new value

## Utility
- [ ] Add `getValue()` function to return current scaled value
- [ ] Add basic debouncing / smoothing