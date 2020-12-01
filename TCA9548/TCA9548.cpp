#include "TCA9548.h"

TCA9548::TCA9548() : i2c(I2C_SDA,I2C_SCL) {}
TCA9548::TCA9548(PinName i2c_sda, PinName i2c_scl) : i2c(i2c_sda,i2c_scl) {}

void TCA9548::i2cWrite8(uint8_t addr, char data){
    char packet[1] ={static_cast<char>(data &  0xFF)}; 
    i2c.write(addr,packet,1,false); //only write address and data ~ no reg
    //wait(0.01);
}

bool TCA9548::init(uint8_t iCh){
    i2c.frequency(400000);
    channel = iCh;
    i2cWrite8(DEV_ADDR,(char)iCh);
    return true;
}

bool TCA9548::init(){
    i2c.frequency(400000);
    channel = 0;
    i2cWrite8(DEV_ADDR,0);
    //wait(0.05);
    return true;
}

void TCA9548::select_I2C(uint8_t newCh){
    channel = newCh;
    i2cWrite8(DEV_ADDR,(char)newCh);
    //wait(0.05);
}