 
set(HALL_SOURCES  pin_encoder.cpp) 
add_library(pin_encoder STATIC ${HALL_SOURCES})
target_link_libraries(pin_encoder  mbed-os   )
set(HALL_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(pin_encoder PUBLIC ${HALL_HEADER_DIR})