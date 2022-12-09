/*
 * Copyright (c) 2006-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

 static BufferedSerial pc(USBTX, USBRX,9600);
 static BufferedSerial uart(D1, D0,9600);

int main()
{
      char msg[] = "Echoes back to the screen anything you type\n";
     char buff;
     char buff1;
      pc.write(msg, sizeof(msg));
    while (1) {
        pc.read(&buff, 1); 
        uart.write(&buff, 1);
        ThisThread::sleep_for(100);
        if (uart.readable()) { uart.read(&buff1, 1);
                                pc.write(&buff1, 1);}
        
    }
}