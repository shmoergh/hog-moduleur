file(GLOB FILES *.cpp *.h)

add_library(mcp48x2 ${FILES})

target_link_libraries(mcp48x2 
                        pico_stdlib
                        hardware_spi)

target_include_directories(mcp48x2 PUBLIC ./)