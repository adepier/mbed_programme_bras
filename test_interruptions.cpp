/*
 * Copyright (c) 2017-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h" 
 #include "pin_encoder.h"  
encoder_values encoder1;
encoder_values encoder2;
encoder_values encoder3;
encoder_values encoder4;
encoder_values encoder5; 


 //L452RE
 #define I2C_SDA PB_9
 #define I2C_SCL PB_8
DigitalInOut mypin0(PB_3,PIN_OUTPUT,PullDown,0); 

pin_encoder counter1_1(PA_5,1,encoder1 );
pin_encoder counter1_2(PA_6,2,encoder1 );
pin_encoder counter2_1(PA_7,1,encoder2 );
pin_encoder counter2_2(PB_2,2,encoder2 );
pin_encoder counter3_1(PB_1,1,encoder3 );
pin_encoder counter3_2(PB_15,2,encoder3 );
pin_encoder counter4_1(PB_14,1,encoder4 );
pin_encoder counter4_2(PB_13,2,encoder4 );
pin_encoder counter5_1(PB_10,1,encoder5 );
pin_encoder counter5_2(PB_4,2,encoder5 );
CAN can1(PA_11, PA_12);

 //L432KC
//  #define I2C_SDA D0
//  #define I2C_SCL D1

// DigitalInOut mypin0(PA_0,PIN_OUTPUT,PullDown,0); 

// pin_encoder counter1_1(D2,1,encoder1 );
// pin_encoder counter1_2(D3,2,encoder1 );
// pin_encoder counter2_1(D4,1,encoder2 );
// pin_encoder counter2_2(D5,2,encoder2 );
// pin_encoder counter3_1(D6,1,encoder3 );
// pin_encoder counter3_2(D9,2,encoder3 );
// pin_encoder counter4_1(D10,1,encoder4 );
// pin_encoder counter4_2(D11,2,encoder4 );
// pin_encoder counter5_1(D12,1,encoder5 );
// pin_encoder counter5_2(D13,2,encoder5 );


I2C i2c(I2C_SDA, I2C_SCL);
// pin_encoder counter6_2(PC_4,1,encoder5 );
// pin_encoder counter6_2(PC_6,2,encoder5 );
 
int test;
int returnCode, address, numberOfDevices = 0;
char outByte = 1;// 0xAA;

int main()
{
  //TEST I2C
  printf("\r\nI2C Bus Scanner\r\n");
    numberOfDevices = 0;
    for(address = 1; address < 127; address++ ){
        returnCode = i2c.write((address << 1),&outByte,1,0); 
        if (returnCode == 0)
        {
           printf("I2C device found at address 0x%X\r\n", address); 
          numberOfDevices++;
        }
    
    }
    if (numberOfDevices == 0){
         printf("No I2C devices found\r\n");
    }
    else{
         printf("Number of devices: %d\r\n",numberOfDevices);
    }
 
//TEST encodeurs
    
     
    while (1) {
         printf("Count1 :   %d:   %d:   %d\n", encoder1.count,encoder1.tic_backward,encoder1.tic_forward);
         printf("Count2 :   %d:   %d:   %d\n", encoder2.count,encoder2.tic_backward,encoder2.tic_forward);
         printf("Count3 :   %d:   %d:   %d\n", encoder3.count,encoder3.tic_backward,encoder3.tic_forward);
         printf("Count4 :   %d:   %d:   %d\n", encoder4.count,encoder4.tic_backward,encoder4.tic_forward);
         printf("Count5 :   %d:   %d:   %d\n", encoder5.count,encoder5.tic_backward,encoder5.tic_forward);
          mypin0.input();
        //  mypin1.input();
         printf("pin :   %d:     \n", mypin0.read()  );
        // printf("Count5 so far 1: %d  / 2 : %d\n\n\n", counter5.read1(), counter5.read2());
         printf("\n\n\n");
        ThisThread::sleep_for(5000);
    }
};