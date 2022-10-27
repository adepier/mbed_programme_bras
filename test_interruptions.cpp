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
// MyInterrupt counter4(D4 );
// MyInterrupt counter5(D5 );
// MyInterrupt counter6(D6 );
// MyInterrupt counter7(D7 );
// MyInterrupt counter8(D8 );
// MyInterrupt counter9(D9 );
// MyInterrupt counter10(D10 );
// MyInterrupt counter11(D11 );
// MyInterrupt counter12(D12 );
// MyInterrupt counter13(D13 );
//
// MyInterrupt counterA0(A0);
// MyInterrupt counterA1(A1);
// MyInterrupt counterA2(A2);
// MyInterrupt counterA3(A3);
// MyInterrupt counterA4(A4);
// MyInterrupt counterA5(A5);
// MyInterrupt counterA6(A6);
// MyInterrupt counterA7(A7);

int test;

int main()
{
    while (1) {
         printf("Count1 :   %d:   %d:   %d\n", encoder1.count,encoder1.tic_backward,encoder1.tic_forward);
         printf("Count1 :   %d:   %d:   %d\n", encoder2.count,encoder2.tic_backward,encoder2.tic_forward);
         printf("Count1 :   %d:   %d:   %d\n", encoder3.count,encoder3.tic_backward,encoder3.tic_forward);
         printf("Count1 :   %d:   %d:   %d\n", encoder4.count,encoder4.tic_backward,encoder4.tic_forward);
         printf("Count1 :   %d:   %d:   %d\n", encoder5count,encoder5.tic_backward,encoder5.tic_forward);
//  printf("Count3 :   %d  \n", counter3.read1());
//  printf("Count4 :   %d  \n", counter4.read1());
//  printf("Count5 :   %d  \n", counter5.read1());
//  printf("Count6 :   %d  \n", counter6.read1());
//  printf("Count7 :   %d  \n", counter7.read1());
//  printf("Count8 :   %d  \n", counter8.read1());
//  printf("Count9 :   %d  \n", counter9.read1());
//  printf("Count10 :   %d  \n", counter10.read1());
//  printf("Count11 :   %d  \n", counter11.read1());
//  printf("Count12 :   %d  \n", counter12.read1());
//   printf("Count13 :   %d  \n", counter13.read1());

// printf("Count A0 :   %d  \n", counterA0.read1());
// printf("Count A1 :   %d  \n", counterA1.read1());
// printf("Count A2 :   %d  \n", counterA2.read1());
// printf("Count A3 :   %d  \n", counterA3.read1());
// printf("Count A4 :   %d  \n", counterA4.read1());
// printf("Count A5 :   %d  \n", counterA5.read1());
// printf("Count A6 :   %d  \n", counterA6.read1());
// printf("Count A7 :   %d  \n", counterA7.read1());


        // printf("Count5 so far 1: %d  / 2 : %d\n\n\n", counter5.read1(), counter5.read2());
         printf("\n\n\n");
        ThisThread::sleep_for(2000);
    }
};