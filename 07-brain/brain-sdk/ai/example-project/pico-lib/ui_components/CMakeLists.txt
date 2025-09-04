file(GLOB FILES *.cpp *.h)

add_library(ui_components ${FILES})

target_link_libraries(ui_components 
                        pico_stdlib
                        hardware_gpio)

target_include_directories(ui_components PUBLIC ./)