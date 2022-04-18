 
set(HALL_SOURCES hall_driven_motor.cpp) 
add_library(hall_driven_motor STATIC ${HALL_SOURCES})
target_link_libraries(hall_driven_motor Adafruit_PWMServoDriver  mbed-os NamedType Arduino-PID-Library   )
set(HALL_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(hall_driven_motor PUBLIC ${HALL_HEADER_DIR})