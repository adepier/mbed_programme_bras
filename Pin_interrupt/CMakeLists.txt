 
set(HALL_SOURCES  Pin_interrupt.cpp) 
add_library(Pin_interrupt STATIC ${HALL_SOURCES})
target_link_libraries(Pin_interrupt  mbed-os   )
set(HALL_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(Pin_interrupt PUBLIC ${HALL_HEADER_DIR})