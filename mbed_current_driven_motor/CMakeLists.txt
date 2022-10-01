 
set(HALL_SOURCES mbed_current_driven_motor.cpp) 
add_library(mbed_current_driven_motor STATIC ${HALL_SOURCES})
target_link_libraries(mbed_current_driven_motor mbed_PWMServoDriver  mbed-os  mbed_INA3221   )
set(HALL_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(mbed_current_driven_motor PUBLIC ${HALL_HEADER_DIR})