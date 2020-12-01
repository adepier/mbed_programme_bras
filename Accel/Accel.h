
#ifndef _Accel_H
#define _Accel_H

#include "mbed.h"
#include "MPU6050/MPU6050.h"
#include "TCA9548/TCA9548.h"
 
#define M_PI 3.14159265358979323846

class Accel {
 public:
  Accel(uint8_t pI2c_channel ,  uint8_t i2c_address,int pOffset,char pPlan_X_Y_Z, int *pCalibation, char pOrientation);
  void begin(void); 
  double lecture_angle(void);
  bool testConnection(void);
  void setXAccelOffset(int16_t pOffset);
  void setYAccelOffset(int16_t pOffset);
  void setZAccelOffset(int16_t pOffset);

 private:
  I2C i2c;
  MPU6050   accelgyro ;
  char plan_X_Y_Z; 
  char orientation;
  int offset;
  uint8_t i2c_channel; 
  TCA9548 multiplexer_i2c;
  int calibation[6];
};

#endif