file(GLOB FILES *.cpp *.h)

add_library(utils ${FILES})

target_link_libraries(utils 
                        pico_stdlib)

target_include_directories(utils PUBLIC ./)