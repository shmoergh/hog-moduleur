add_library(brain-io
    brain-io.c
    brain-io.h
    pulse.cpp
    midi-parser.cpp
    audio-cv-out.cpp
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
    brain-common
    hardware_gpio
    hardware_timer
    hardware_spi
)
target_include_directories(brain-io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
