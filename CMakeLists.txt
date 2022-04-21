# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0


cmake_minimum_required(VERSION 3.19.0)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../mbed-os  CACHE INTERNAL "" )
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "") 
set(APP_TARGET mbed_programme_bras  )


include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

# ajoute les lib
add_subdirectory(${MBED_PATH}  ${CMAKE_CURRENT_BINARY_DIR}/mbed-os )   
add_subdirectory(NamedType)   
add_subdirectory(mbed_PWMServoDriver  )      
add_subdirectory(mbed_hall_driven_motor )   
add_subdirectory(mbed_PID_Library )   
add_subdirectory(mbed_MPU6050 )   
add_subdirectory(mbed_I2Cdev )   

add_executable(${APP_TARGET}
      main.cpp
)  
 
 

 target_sources(${APP_TARGET}
    PRIVATE
)

# premet d'utiliser les lib sans le chemin
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