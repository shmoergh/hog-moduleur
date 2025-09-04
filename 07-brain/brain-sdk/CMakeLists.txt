file(GLOB FILES src/*.cpp src/*.h)

cmake_minimum_required(VERSION 3.22)
include(pico_sdk_import.cmake)
project(brain-sdk VERSION 1.0.0)

# Initialize Pico SDK before adding subdirectories
pico_sdk_init()

# Add subdirectories for libraries and programs
add_subdirectory(lib)
add_subdirectory(programs)
