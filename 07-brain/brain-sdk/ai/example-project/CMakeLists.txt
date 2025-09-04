file(GLOB FILES src/*.cpp src/*.h)

cmake_minimum_required(VERSION 3.22)
include(pico_sdk_import.cmake)
project(hog-midi2cv VERSION 1.0.0)
add_executable(${PROJECT_NAME}
                ${FILES})

pico_sdk_init()

add_subdirectory(./pico-lib)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
                        pico_stdlib
                        hardware_gpio
                        hardware_i2c
                        hardware_spi
                        hardware_adc
                        hardware_uart
                        mcp48x2
                        ringbuffer
                        midi_parser
                        utils)
