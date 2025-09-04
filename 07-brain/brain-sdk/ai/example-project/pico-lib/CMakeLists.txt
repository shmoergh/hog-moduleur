cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


if (NOT EXISTS $ENV{PICO_SDK_PATH})
message(FATAL_ERROR "Pico SDK Path not found, please set correct environment variable PICO_SDK_PATH")
endif ()

include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
pico_sdk_init()

enable_language(C CXX ASM)

add_subdirectory(utils)
add_subdirectory(adsr)
add_subdirectory(mcp48x2)
add_subdirectory(ringbuffer)
add_subdirectory(midi_parser)
add_subdirectory(ui_components)
add_subdirectory(fix16)

project(pico-lib VERSION 1.0)