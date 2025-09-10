add_library(brain-ui
	brain-ui.c
	brain-ui.h
	button.cpp
	led.cpp
	pot-multiplexer.cpp
)
target_include_directories(brain-ui PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	include
	..
	${PICO_SDK_PATH}/src/common/include
	${PICO_SDK_PATH}/src/rp2_common/hardware_gpio/include
	${PICO_SDK_PATH}/src/rp2_common/pico_stdlib/include
)
target_link_libraries(brain-ui
	pico_stdlib
	hardware_adc
	hardware_pwm
)
target_include_directories(brain-ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
