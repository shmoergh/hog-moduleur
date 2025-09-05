add_library(brain-ui brain-ui.c brain-ui.h button.cc)
target_include_directories(brain-ui PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	include
	${PICO_SDK_PATH}/src/common/include
	${PICO_SDK_PATH}/src/rp2_common/hardware_gpio/include
	${PICO_SDK_PATH}/src/rp2_common/pico_stdlib/include
)
target_link_libraries(brain-ui pico_stdlib)
target_include_directories(brain-ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
