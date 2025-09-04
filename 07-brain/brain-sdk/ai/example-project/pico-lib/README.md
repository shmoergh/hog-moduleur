# Raspberry Pi Pico libraries
Utilities and drivers for RPI Pico. 

## Usage

1. Navigate to your main repo in terminal and clone this repo as a git submodule to your main repo:

```
cd my-pico-project
git submodule add git@github.com:peterzimon/pico-lib.git
git commit -m "Added the damn pico-lib submodule to the project."
git push
```

2. Add the `pico-lib` subdirectory to your main project's `CMakeLists.txt` file:

```
add_subdirectory(./pico-lib)
```

3. Link the libraries you want to use to your main project. Each library's name is the same
as the name of its containing directory, for example:

```
target_link_libraries(${PROJECT_NAME}
                        mcp48x2)
```

Alternatively instead of step 2 and 3, you can use the 
[sample `CMakeLists.txt`](https://github.com/peterzimon/pico-midi2cv/blob/main/CMakeLists.txt.sample) 
from this repo as your main project's CMakeLists.txt file (just don't forget to remove the `.sample` extension).

4. Include the library in your main project, for example:

```
#include <mcp48x2.h>
```


## List of libs
- [mcp48x2](https://github.com/peterzimon/pico-lib/tree/main/mcp48x2): Driver for MCP4802, MCP4812 and MCP4822 DAC's
- [ringbuffer](https://github.com/peterzimon/pico-lib/tree/main/ringbuffer): Ringbuffer utility