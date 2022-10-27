/*
 * Copyright (c) 2017-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

class Counter {
public:
    Counter(PinName pin1 ) : _interrupt1(pin1,PullDown)          // create the InterruptIn on the pin specified to Counter
    {   
        _interrupt1.enable_irq(); 

        _interrupt1.rise(callback(this, &Counter::increment1)); // attach increment function of this counter instance 
    }

    void increment1()
    {
        _count1++;
    }

    int read1()
    {
        return _count1;
    }
    
private:
    InterruptIn _interrupt1;
     
    volatile int _count1; 
};


Counter counter2(D2 );
Counter counter3(D3 );
Counter counter4(D4 );
Counter counter5(D5 );
Counter counter6(D6 );
Counter counter7(D7 );
Counter counter8(D8 );
Counter counter9(D9 );
Counter counter10(D10 );
Counter counter11(D11 );
Counter counter12(D12 );
Counter counter13(D13 );
//
// Counter counterA0(A0);
// Counter counterA1(A1);
// Counter counterA2(A2);
// Counter counterA3(A3);
// Counter counterA4(A4);
// Counter counterA5(A5);
// Counter counterA6(A6);
// Counter counterA7(A7);



int main()
{
    while (1) {
         printf("Count2 :   %d  \n", counter2.read1());
 printf("Count3 :   %d  \n", counter3.read1());
 printf("Count4 :   %d  \n", counter4.read1());
 printf("Count5 :   %d  \n", counter5.read1());
 printf("Count6 :   %d  \n", counter6.read1());
 printf("Count7 :   %d  \n", counter7.read1());
 printf("Count8 :   %d  \n", counter8.read1());
 printf("Count9 :   %d  \n", counter9.read1());
 printf("Count10 :   %d  \n", counter10.read1());
 printf("Count11 :   %d  \n", counter11.read1());
 printf("Count12 :   %d  \n", counter12.read1());
  printf("Count13 :   %d  \n", counter13.read1());

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
}