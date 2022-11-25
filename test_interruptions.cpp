/*
 * Copyright (c) 2017-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h" 
 #include "Pin_interrupt.h" 

Encoder encoder1;
Encoder encoder2;
Encoder encoder3;
Encoder encoder4;
Encoder encoder5;
DigitalIn mypin1(A0);
DigitalIn mypin2(A1);
DigitalIn mypin3(A2);
DigitalIn mypin4(A3);
DigitalIn mypin5(A4);

Pin_interrupt counter1_1(D2,1,encoder1 );
Pin_interrupt counter1_2(D3,2,encoder1 );
Pin_interrupt counter2_1(D4,1,encoder2 );
Pin_interrupt counter2_2(D5,2,encoder2 );
Pin_interrupt counter3_1(D6,1,encoder3 );
Pin_interrupt counter3_2(D9,2,encoder3 );
Pin_interrupt counter4_1(D10,1,encoder4 );
Pin_interrupt counter4_2(D11,2,encoder4 );
Pin_interrupt counter5_1(D12,1,encoder5 );
Pin_interrupt counter5_2(D13,2,encoder5 );


int test;

int main()
{
    while (1) {
         printf("Count1 :   %d:   %d:   %d\n", encoder1.count,encoder1.tic_backward,encoder1.tic_forward);
         printf("Count2 :   %d:   %d:   %d\n", encoder2.count,encoder2.tic_backward,encoder2.tic_forward);
         printf("Count3 :   %d:   %d:   %d\n", encoder3.count,encoder3.tic_backward,encoder3.tic_forward);
         printf("Count4 :   %d:   %d:   %d\n", encoder4.count,encoder4.tic_backward,encoder4.tic_forward);
         printf("Count5 :   %d:   %d:   %d\n", encoder5.count,encoder5.tic_backward,encoder5.tic_forward);
        // printf("Count5 so far 1: %d  / 2 : %d\n\n\n", counter5.read1(), counter5.read2());
         printf("\n\n\n");
        ThisThread::sleep_for(2000);
    }
};