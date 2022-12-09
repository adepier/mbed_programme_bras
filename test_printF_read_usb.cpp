/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

// Create a serial object
static BufferedSerial pc(USBTX, USBRX);
static BufferedSerial uart(D1, D0,9600);

 //The constructor takes in RX, and TX pin respectively.
//These pins, for this example, are defined in mbed_app.json
CAN can1(D10, D2);
int flag=0;

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
            uart.write(buffer1,10);
            flag=1;
            ThisThread::sleep_for(1000);
            while (flag==1) {
            if (uart.readable()) {
                ThisThread::sleep_for(1000);
                char buffer2[10];
                puartc1.read(buffer2, 10);
                printf("\nI got 2 '%s'\n", buffer2);
                flag=0;
                }
            }            
        }
    }
}

 