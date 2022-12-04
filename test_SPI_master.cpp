/*
 * Copyright (c) 2006-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

SPI spi(A6, A5, A4); // mosi, miso, sclk
 
DigitalOut cs(A3);

int main()
{
    // Chip must be deselected
    cs = 1;

    // Setup the spi for 8 bit data, high steady state clock,
    // second edge capture, with a 1MHz clock rate
    spi.format(8, 3);
    spi.frequency(1000000);
    cs = 0;
while(1)
    {// Select the device by seting chip select low
    

    // Send 0x8f, the command to read the WHOAMI register
   // spi.write(0x8F);

    // Send a dummy byte to receive the contents of the WHOAMI register
    int whoami = spi.write("toto");
    //printf("WHOAMI register = 0x%X\n", whoami);

    // Deselect the device
    
    ThisThread::sleep_for(1000);}
    cs = 1;
}