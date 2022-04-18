 
set(PWM_SOURCES Adafruit_PWMServoDriver.cpp) 
add_library(Adafruit_PWMServoDriver STATIC ${PWM_SOURCES})
target_link_libraries( Adafruit_PWMServoDriver mbed-os)

set(PWM_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(Adafruit_PWMServoDriver PUBLIC ${PWM_HEADER_DIR})