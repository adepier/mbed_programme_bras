/*
 * Copyright (c) 2017-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

class Counter {
public:
    Counter(PinName pin1,PinName pin2) : _interrupt1(pin1) ,_interrupt2(pin2)         // create the InterruptIn on the pin specified to Counter
    {   
        _interrupt1.enable_irq();
        _interrupt2.enable_irq();

        _interrupt1.rise(callback(this, &Counter::increment1)); // attach increment function of this counter instance
        _interrupt2.rise(callback(this, &Counter::increment2)); // attach increment function of this counter instance
    }

    void increment1()
    {
        _count1++;
    }

    int read1()
    {
        return _count1;
    }
    void increment2()
    {
        _count2++;
    }

    int read2()
    {
        return _count2;
    }
private:
    InterruptIn _interrupt1;
    InterruptIn _interrupt2;
    volatile int _count1;
    volatile int _count2;
};


Counter counter1(D2,D3);
Counter counter2(D4,D5);
Counter counter3(D6,D12);
 Counter counter4(D9,D11);
 Counter counter5(A4,A3);


int main()
{
    while (1) {
        printf("Count1 so far 1: %d  / 2 : %d\n", counter1.read1(), counter1.read2());
        printf("Count2 so far 1: %d  / 2 : %d\n", counter2.read1(), counter2.read2());
        printf("Count3 so far 1: %d  / 2 : %d\n", counter3.read1(), counter3.read2());
         printf("Count4 so far 1: %d  / 2 : %d\n", counter4.read1(), counter4.read2());
        // printf("Count5 so far 1: %d  / 2 : %d\n\n\n", counter5.read1(), counter5.read2());
        printf("\n\n\n");
        ThisThread::sleep_for(2000);
    }
}