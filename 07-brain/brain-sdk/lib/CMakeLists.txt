# Find and build all subdirectories in lib/
file(GLOB children RELATIVE ${CMAKE_CURRENT_LIST_DIR} *)
foreach(child ${children})
    if(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${child})
        add_subdirectory(${child})
    endif()
endforeach()
