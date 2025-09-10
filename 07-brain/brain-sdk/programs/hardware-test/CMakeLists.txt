cmake_minimum_required(VERSION 3.22)
project(hardware-test)
add_executable(hardware-test
    main.cpp
    led_test.cpp
    button_test.cpp
    potmux_test.cpp
    pulse_test.cpp
    midi_test.cpp
    audio_cv_out_test.cpp
    audio_cv_in_test.cpp
)
target_link_libraries(hardware-test
    pico_stdlib
    brain-common
    brain-ui
    brain-io
    hardware_spi
    hardware_gpio
)
pico_enable_stdio_usb(hardware-test 1)
pico_enable_stdio_uart(hardware-test 1)
pico_add_extra_outputs(hardware-test)
