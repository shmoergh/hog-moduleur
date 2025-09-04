# Todo: Create new-program.sh generator script

## 1. Script basics
- [ ] Create a shell script named `new-program.sh` inside the `scripts/` folder
- [ ] Make the script executable (`chmod +x scripts/new-program.sh`)
- [ ] The script must take **one argument**: the program name
      Example: `./scripts/new-program.sh blinky`

## 2. Folder + file creation
- [ ] Create a new folder under `programs/` with the given program name
      Example: `programs/blinky/`
- [ ] Inside that folder, create:
  - [ ] `CMakeLists.txt`
  - [ ] `main.cpp`

## 3. Boilerplate CMakeLists.txt
- [ ] The generated `CMakeLists.txt` should:
  - [ ] Define a CMake project with the program name
  - [ ] Call `add_executable(<name> main.cpp)`
  - [ ] Link against `pico_stdlib`, `brain_ui` and `brain_io`
  - [ ] Enable USB output and UART by default:
    ```cmake
    pico_enable_stdio_usb(<name> 1)
    pico_enable_stdio_uart(<name> 1)
    ```
  - [ ] Add UF2 output with `pico_add_extra_outputs(<name>)`

## 4. Boilerplate main.cpp
- [ ] The generated `main.cpp` should implement a **blinky program**:
  - [ ] Include `pico/stdlib.h`
  - [ ] Define `const uint LED_PIN = PICO_DEFAULT_LED_PIN;`
  - [ ] Initialize stdio and GPIO
  - [ ] In an infinite loop, toggle the LED every 500 ms

## 5. Script behavior
- [ ] If no program name is provided, print usage:
      `Usage: ./scripts/new-program.sh <program-name>`
- [ ] If the program folder already exists, abort with an error message
- [ ] After creating files, print success message with the new program path

## 6. Documentation
- [ ] Add usage instructions to `docs/README.md`:
  - How to run the script
  - Example: `./scripts/new-program.sh my_app`
  - How to build: `cd build && cmake .. && make -j4`