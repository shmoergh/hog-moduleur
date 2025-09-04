file(GLOB FILES *.cpp *.h)

add_library(adsr ${FILES})

target_link_libraries(adsr pico_stdlib)

target_include_directories(adsr PUBLIC ./)