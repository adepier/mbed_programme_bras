/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

// Create a serial object
static BufferedSerial pc(USBTX, USBRX);





int main(void)
{   
   double pi = 3.14159265359;
    
    string s = "Echoes back to the screen anything you type\0";
    char buffer[s.length() + 1];
    strcpy(buffer, s.c_str()); 
    printf("%s %f\n", buffer,pi);  
    while (1) {
        if (pc.readable()) {
            ThisThread::sleep_for(1000);
            char buffer1[10];
            pc.read(buffer1, 10);
            printf("\nI got '%s'\n", buffer1);
            
        }
    }
}

 