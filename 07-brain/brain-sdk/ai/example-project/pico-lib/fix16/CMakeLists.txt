file(GLOB FILES *.cpp *.h)

add_library(fix16 ${FILES})

target_link_libraries(fix16
                        pico_stdlib)

target_include_directories(fix16 PUBLIC ./)