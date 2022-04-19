# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0


cmake_minimum_required(VERSION 3.19.0)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../mbed-os  CACHE INTERNAL "" )
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "") 
set(APP_TARGET mbed_programme_bras  )


include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(${MBED_PATH}  ${CMAKE_CURRENT_BINARY_DIR}/mbed-os )   
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/NamedType)   
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mbed_PWMServoDriver ${CMAKE_CURRENT_BINARY_DIR}/mbed_PWMServoDriver )      
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mbed_hall_driven_motor ${CMAKE_CURRENT_BINARY_DIR}/mbed_hall_driven_motor )   
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mbed_PID_Library ${CMAKE_CURRENT_BINARY_DIR}/mbed_PID_Library )   
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mbed_MPU6050 ${CMAKE_CURRENT_BINARY_DIR}/mbed_MPU6050 )   
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mbed_I2Cdev ${CMAKE_CURRENT_BINARY_DIR}/mbed_I2Cdev )   

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
    # MPU6050/MPU6050.h
    # MPU6050/MPU6050.cpp
    # MPU6050/MPU6050_6Axis_MotionApps20.h
    # MPU6050/I2Cdev/I2Cdev.cpp
    # MPU6050/helper_3dmath.h
    # mbed_hall_driven_motor/mbed_hall_driven_motor.cpp
    # mbed_hall_driven_motor/mbed_hall_driven_motor.h
    # mbed_PWMServoDriver/mbed_PWMServoDriver.cpp
    # mbed_PWMServoDriver/mbed_PWMServoDriver.h 
    # PID/PID_v1.h
    # PID/PID_v1.cpp 
)


############################## 

target_link_libraries(${APP_TARGET} 
    PUBLIC 
        mbed-os
        mbed_PWMServoDriver
        mbed_hall_driven_motor
        NamedType
        mbed_PID_Library
        mbed_MPU6050
        mbed_I2Cdev )

mbed_set_post_build(${APP_TARGET})

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

# Generate compile commands database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")