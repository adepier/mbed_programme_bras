/*
 * Copyright (c) 2017-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */


#include "mbed.h"

DigitalIn mypin1(A0);
DigitalIn mypin2(A1);
DigitalIn mypin3(A2);
DigitalIn mypin4(A3);
DigitalIn mypin5(A6);

int main()
{
    // check that mypin object is initialized and connected to a pin
    if (mypin1.is_connected()) { printf("mypin is initialized and connected!\n\r"); }
    if (mypin2.is_connected()) { printf("mypin is initialized and connected!\n\r"); }
    if (mypin3.is_connected()) { printf("mypin is initialized and connected!\n\r"); }
    if (mypin4.is_connected()) { printf("mypin is initialized and connected!\n\r"); }
    if (mypin5.is_connected()) { printf("mypin is initialized and connected!\n\r"); }

    // Optional: set mode as PullUp/PullDown/PullNone/OpenDrain
    mypin1.mode(PullUp);
    mypin2.mode(PullUp);
    mypin3.mode(PullUp);
    mypin4.mode(PullUp);
    mypin5.mode(PullDown);

    while (1) {
        
        
        printf("mypin1.read() = %d \n\r", mypin1.read());
        printf("mypin2.read() = %d \n\r", mypin2.read());
        printf("mypin3.read() = %d \n\r", mypin3.read());
        printf("mypin4.read() = %d \n\r", mypin4.read());
        printf("mypin5.read() = %d \n\r", mypin5.read());
        printf(" \n\r" );
        ThisThread::sleep_for(500);
    }
}
