cmake_minimum_required(VERSION 3.22)
project(midi2cv)

file(GLOB MIDI2CV_SRC
    src/*.cpp
)

add_executable(midi2cv ${MIDI2CV_SRC})

target_include_directories(midi2cv PRIVATE src)

set_target_properties(midi2cv PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

target_link_libraries(midi2cv
    pico_stdlib
    brain-common
    brain-ui
    brain-io)

pico_enable_stdio_usb(midi2cv 1)
pico_enable_stdio_uart(midi2cv 1)
pico_add_extra_outputs(midi2cv)
