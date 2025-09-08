add_library(brain-io
    brain-io.c
    brain-io.h
    pulse.cpp
    midi_parser.cpp
)
target_include_directories(brain-io PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	include
	..
	${PICO_SDK_PATH}/src/common/include
	${PICO_SDK_PATH}/src/rp2_common/hardware_gpio/include
	${PICO_SDK_PATH}/src/rp2_common/pico_stdlib/include
)
target_link_libraries(brain-io
    pico_stdlib
    hardware_gpio
    hardware_timer
)
target_include_directories(brain-io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
