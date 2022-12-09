// Original program found here: http://playground.arduino.cc/Main/I2cScanner
// Modified by Domen Ornik, 4.5.2015
 
#include "mbed.h"
 
#define I2C_SDA PA_10
#define I2C_SCL PA_9
 
I2C i2c(I2C_SDA , I2C_SCL );  
int returnCode, address, numberOfDevices = 0;
char outByte = 0xAA;
 
int main() {
     
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
        
}