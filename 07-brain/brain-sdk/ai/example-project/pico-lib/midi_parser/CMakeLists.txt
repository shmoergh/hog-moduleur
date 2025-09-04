file(GLOB FILES *.cpp *.h)

add_library(midi_parser ${FILES})

target_link_libraries(midi_parser pico_stdlib)

target_include_directories(midi_parser PUBLIC ./)