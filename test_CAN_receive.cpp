/*
 * Copyright (c) 2017-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#if !DEVICE_CAN
#error [NOT_SUPPORTED] CAN not supported for this target
#endif


#include "mbed.h"

/** The constructor takes in RX, and TX pin respectively.
  * These pins, for this example, are defined in mbed_app.json
  */
  
CAN can(USBRX,USBTX);

int main()
{
    printf("main()\n");
    CANMessage msg;
    while (1) {
        printf("loop()\n");
        if (can.read(msg)) {
            printf("Message received: %d\n", msg.data[0]);
      
        }
        ThisThread::sleep_for(200);
    }
}