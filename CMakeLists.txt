# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0


cmake_minimum_required(VERSION 3.19.0)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../mbed-os  CACHE INTERNAL "" )
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "") 
set(APP_TARGET programme_bras  )

include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(${MBED_PATH}  ${CMAKE_CURRENT_BINARY_DIR}/mbed-os )   
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/NamedType)   

add_executable(${APP_TARGET}
      main.cpp
    # test_coude.cpp
    #test_poignet.cpp
    #test_epaule_a_plat.cpp
    #test_epaule_haut.cpp
    #test_epaule_coude_syncrho
    #test_epaule_poignet_syncrho
    #test_printF.cpp
)  
 
 
###############################Ajout lib

 target_sources(${APP_TARGET}
    PRIVATE
    MPU6050/MPU6050.h
    MPU6050/MPU6050.cpp
    MPU6050/MPU6050_6Axis_MotionApps20.h
    MPU6050/I2Cdev/I2Cdev.cpp
    MPU6050/helper_3dmath.h
    hall_driven_motor/hall_driven_motor.cpp
    hall_driven_motor/hall_driven_motor.h
    adafruit_pwmservodriver/Adafruit_PWMServoDriver.cpp
    adafruit_pwmservodriver/Adafruit_PWMServoDriver.h 
    PID/PID_v1.h
    PID/PID_v1.cpp 
)


############################## 
target_link_libraries(${APP_TARGET} 
    PRIVATE 
        mbed-os )

mbed_set_post_build(${APP_TARGET})

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

# Generate compile commands database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")