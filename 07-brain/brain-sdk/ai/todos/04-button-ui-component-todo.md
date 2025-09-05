# Todo: Implement Button Component (brain-io library)

## 1. File structure
- [x] Create new files:
  - `lib/brain-io/include/brain-io/button.h`
  - `lib/brain-io/button.cc`
- [x] Update `lib/brain-io/CMakeLists.txt` to include `button.cc` in the build.

## 2. Class design
- [x] Create a C++ class `Button` inside namespace `brain::io`.
- [x] Constructor should take:
  - GPIO pin number
  - Optional debounce time (default 50 ms)
  - Optional long press threshold (default 500 ms)
  - Optional double-tap threshold (default 300 ms)
- [x] Store internal state:
  - `uint gpio_pin_`
  - `bool is_pressed_`
  - `absolute_time_t last_press_time_`
  - `absolute_time_t last_release_time_`
  - `absolute_time_t last_tap_time_`
  - Debounce threshold, long press threshold, double-tap threshold

## 3. GPIO setup
- [x] Add `Init()` method:
  - Configure the GPIO pin as input with pull-up or pull-down.
  - Initialize state variables.

## 4. Event handling
- [x] Add `Update()` method to be called periodically (e.g., every 1–10 ms in main loop or timer):
  - Read GPIO pin state.
  - Apply debounce logic (ignore changes shorter than debounce threshold).
  - Detect rising edge (press) and falling edge (release).
  - Detect single tap, double tap, long press.

## 5. Callbacks
- [x] Allow registration of callbacks:
  - `SetOnPress(std::function<void()> callback)`
  - `SetOnRelease(std::function<void()> callback)`
  - `SetOnSingleTap(std::function<void()> callback)`
  - `SetOnDoubleTap(std::function<void()> callback)`
  - `SetOnLongPress(std::function<void()> callback)`
- [ ] Trigger callbacks inside `Update()` when events are detected.

## 6. Example usage
- [ ] Create a minimal example program in `programs/sandbox/`:
  - Setup a `Button` on GPIO 15.
  - Print messages on press/release/single/double/long press.

## 7. Documentation
- [ ] In `button.h`, document class and methods with Doxygen-style comments.
- [ ] In `docs/README.md`, briefly describe the `Button` component.