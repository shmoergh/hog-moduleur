# Brain Common Library

This library provides shared definitions and constants used across all Brain modules.

## Contents

- **brain-gpio-setup.h**: GPIO pin assignments for the Brain hardware
- **brain_common.h**: Common constants and utility definitions

## Usage

To use this library in your project, simply link against `brain-common`:

```cmake
target_link_libraries(your-target
    brain-common
    # ... other dependencies
)
```

Then include the headers you need:

```cpp
#include "brain-gpio-setup.h"        // For GPIO pin definitions
#include "brain_common.h"            // For common constants and utilities
```

## Benefits

1. **Centralized Configuration**: All GPIO pins and common constants are defined in one place
2. **Easy Maintenance**: Changes to pin assignments only need to be made in one location
3. **Type Safety**: Using `constexpr` constants instead of `#define` macros provides better type checking
4. **Proper Dependencies**: CMake handles include paths automatically through library dependencies
5. **Interface Library**: Zero overhead - all definitions are compile-time constants
