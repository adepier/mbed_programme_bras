/*
 * Copyright (c) 2017-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

class Counter {
public:
    Counter(PinName pin1) : _interrupt1(pin1) ,_interrupt1(pin2)         // create the InterruptIn on the pin specified to Counter
    {
        _interrupt1.rise(callback(this, &Counter::increment)); // attach increment function of this counter instance
        _interrupt2.rise(callback(this, &Counter::increment)); // attach increment function of this counter instance
    }

    void increment()
    {
        _count++;
    }

    int read()
    {
        return _count;
    }

private:
    InterruptIn _interrupt1;
    InterruptIn _interrupt2;
    volatile int _count;
};

Counter counter1(PA_0,PB_0);
Counter counter1(PA_1,PB_1);

int main()
{
    while (1) {
        printf("Count so far: %d\n", counter1.read());
        ThisThread::sleep_for(2000);
    }
}