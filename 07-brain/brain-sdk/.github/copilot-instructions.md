# Copilot Instructions for Hog Moduleur Brain Module

## Project Context
- This project is the **Brain module** of the Hog Moduleur synthesizer.
- The Brain is built around a **Raspberry Pi Pico (or Pico 2)** microcontroller.
- It provides **universal digital I/O** for Eurorack synthesizers and runs custom firmware written in **C++ with the Pico SDK**.
- Factory programs include: MIDI-to-CV converter, sequencer/arpeggiator, delay, reverb, drum machine, etc.

## Hardware Features
Copilot should assume the following hardware environment:
- **Inputs**
    - 2x analog inputs (CV/audio) → Pico ADC
    - 1x pulse input (digital GPIO, with transistor switch)
    - 1x dedicated MIDI input (special MIDI circuitry)
    - 3x potentiometers (multiplexed, read via Pico ADC)
    - 2x pushbuttons
- **Outputs**
    - 2x analog outputs → MCP4822 DAC (AC/DC coupling selectable via CD4053 switch)
    - 1x pulse output (digital GPIO with transistor switch)
    - 6x LEDs, driven via transistors, with brightness controlled by Pico PWM

## Code Style & Structure
- **Language**: C++ (Pico SDK)
- **Project structure**: Modular, separated into libraries:
    - `lib/brain-io`: low-level input/output handling (ADC, DAC, GPIO, MIDI, PWM, etc.)
    - `lib/brain-ui`: higher-level abstractions for pots, buttons, LEDs
    - `programs/`: individual firmware apps (e.g. midi-to-cv, sequencer, fx)
- **Object-oriented approach**:
    - Each hardware component (button, pot, LED, DAC channel, etc.) should be encapsulated in a class.
    - Prefer callbacks for event-driven code (e.g. button press/release, end of LED blink).
- **Naming conventions**:
    - 4-space indent, max 100 cols, UTF-8, Unix newlines.
    - Filenames: `lower_case_with_underscores.{h,cpp}`.
    - Folder names: `kebab-case` (e.g., `brain-io`, `brain-dsp`).
    - Types/classes/enums: `camelCase`.
    - Functions/methods: `camelCase`.
    - Variables: `lower_case_with_underscores`; class members end with `_`.
    - Constants: `kPascalCase`.
    - Macros: `ALL_CAPS_WITH_UNDERSCORES` (avoid; prefer `constexpr`/`enum`).
    - No exceptions, no RTTI by default. No dynamic allocation in ISRs.
    - `const` and `constexpr` aggressively. Prefer `enum class` over `#define`.
    - One `#include` per line. Order: C system → C++ std → third-party → project.
    - Headers are self-contained & guarded. `.cpp` mirrors header order.
    - Public APIs documented with brief Doxygen comments.
- **Formatting**: Follow Google C++ style, tabs instead of spaces for indentation.

## Expectations for Copilot
When writing code for this project, Copilot should:
1. Assume access to **Pico SDK** (CMake build system).
2. Use **object-oriented C++** with clean, modular classes.
3. Expose **callback functions** for events (button actions, pot changes, blink finished, etc.).
4. Provide **update() methods** for components that need polling in the main loop.
5. Keep implementations minimal but extensible (no overengineering).
6. Favor **readability and testability** over micro-optimizations unless DSP/audio performance is required.
7. Go in small chunks so I can review and make sure everything looks good.
8. Remove all temporary debug/build tasks from .vscode/tasks.json after use. Only keep permanent, project-wide tasks.
9. Always update todo lists by checking off finished tasks as work progresses.
10. In agent mode, apply changes to a single file directly without asking for permission. Ask for permission only when editing multiple files at once.
11. Always check both the header and implementation files of all C++ classes when making changes or reviewing code.
12. When you create a new component make sure to doublecheck if the related includes are correctly linked in the corresponding CMakeLists.txt files.