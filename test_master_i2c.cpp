/*
 * Copyright (c) 2014-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

// Read temperature from LM75BD

I2C i2c(I2C_SDA, I2C_SCL);
#define SLAVE_ADDR 0xA0 

int main()
{
    char cmd[64];
    while (1) {
        cmd[0] = 0x01;
        cmd[1] = 0x00;
        i2c.write(SLAVE_ADDR, cmd, 2); 
        ThisThread::sleep_for(500); 
        cmd[0] = 0x00;
        i2c.write(SLAVE_ADDR, cmd, 1);
        i2c.read(SLAVE_ADDR, cmd, 64);

         printf("get from slave: %s\n", cmd); 
    }
}