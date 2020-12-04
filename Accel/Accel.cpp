#include <Accel/Accel.h>
#include "MPU6050/MPU6050.h" 
#include "TCA9548/TCA9548.h"
#define I2C_SDA PB_7
#define I2C_SCL PB_6
#define NB_READ 10


Accel::Accel(uint8_t pI2c_channel , uint8_t i2c_address,int pOffset,char pPlan_X_Y_Z, int *pCalibation, int pOrientation) 
        : i2c(I2C_SDA,I2C_SCL), accelgyro(i2c_address), multiplexer_i2c()  {
 
  
  plan_X_Y_Z = pPlan_X_Y_Z;
  offset =pOffset;
  i2c_channel= pI2c_channel;
  calibation[0] = pCalibation[0];
  calibation[1] = pCalibation[1];
  calibation[2] = pCalibation[2]; 
  orientation = pOrientation;
  
}
 
void Accel::begin(void){
   // change de bus I2C  (TCA9648A) 
    multiplexer_i2c.init(i2c_channel);
    accelgyro.initialize() ;  
    
}
void Accel::setXAccelOffset(int16_t offset){
     // change de bus I2C  (TCA9648A) 
      multiplexer_i2c.select_I2C(i2c_channel);
        accelgyro.setXAccelOffset(offset);
  }
  void Accel::setYAccelOffset(int16_t offset){
     // change de bus I2C  (TCA9648A) 
      multiplexer_i2c.select_I2C(i2c_channel);
        accelgyro.setYAccelOffset(offset);
  }
    void Accel::setZAccelOffset(int16_t offset){
     // change de bus I2C  (TCA9648A) 
      multiplexer_i2c.select_I2C(i2c_channel);
        accelgyro.setZAccelOffset(offset);
  }
  
double Accel::lecture_angle(void){
 debut:
int16_t ax = 0 , ay = 0, az = 0;//acceleration 

double y_val=0,  x_val=0,z_val=0;
double x_euler=0, y_euler=0, z_euler=0,   angle =0;
  
    
    multiplexer_i2c.select_I2C(i2c_channel);
    //*** lecture de l'ange de l'accéléromètre
    // on lit NB_READ fois l'angle pour en faire la moyenne
    for (size_t i = 0; i < NB_READ; i++)
    {
        //"XYZ" =1
        //"XZY" =2
        if ( orientation ==  1) {accelgyro.getAcceleration(&ax, &ay, &az); }
        if ( orientation ==  2) {accelgyro.getAcceleration(&ax, &az, &ay); }
         



        ax=ax * calibation[0] ;
        ay=ay * calibation[1] ;
        az=az * calibation[2] ;
    
        x_euler = x_euler + atan( ax / sqrt( (ay * ay + az * az))) * 180.0 / M_PI ; //x Pitch
        y_euler = y_euler + atan( ay / sqrt(  (ax * ax + az * az))) * 180.0 / M_PI; //y Roll
        z_euler = z_euler + atan( az / sqrt(  (ax * ax + az * az))) * 180.0 / M_PI; //z y_euleraw
      
       
        wait_us(100); 
    }
    //AngleCoude = map(y_euler_moy / 10,0,83.57,0,90);
        // debugSerial.printf("%c      ", orientation);
        // debugSerial.printf("%i      ", ax);
        // debugSerial.printf("%i      ", ay);
        // debugSerial.printf("%i      ", az);
        // debugSerial.printf("\n" ); 
           
     
    if(isnan(x_euler) ) { goto debut;}
    if(isnan(x_euler) == false){x_euler = x_euler / NB_READ ; }
    if(isnan(y_euler) == false){y_euler = y_euler / NB_READ ; }
    if(isnan(z_euler) == false){z_euler = z_euler / NB_READ ; }
       
    //debugSerial.printf("x:%i\t y:%i\t z:%i cal:%i\n ", cx, cy, cz,calibation[0] );
     
        //convertit l'angle sur 360°
      
        //****x_euler
        if (x_euler<0 && y_euler>0){x_val = - x_euler;}
        if (x_euler<0 && y_euler<0){x_val = 180.0 + x_euler;}
        if (x_euler>0 && y_euler<0){x_val = 180.0 + x_euler;}
        if (x_euler>0 && y_euler>0){x_val = 360.0 - x_euler;}
        //***y_euler
        if (y_euler>0 && x_euler<0){y_val =  90.0 - y_euler;}
        if (y_euler<0 && x_euler<0){y_val =  90.0 - y_euler;}
        if (y_euler>0 && x_euler>0){y_val = 270.0 + y_euler;}
        if (y_euler<0 && x_euler>0){y_val = 270.0 + y_euler;}
        //*** z_euler
        if (z_euler>0 && x_euler<0){z_val =  90.0 - z_euler;}
        if (z_euler<0 && x_euler<0){z_val =  90.0 - z_euler;}
        if (z_euler>0 && x_euler>0){z_val = 270.0 + z_euler;}
        if (z_euler<0 && x_euler>0){z_val = 270.0 + z_euler;}
        //on prend la partie de l'axe la plus fiable pour le resultat angle_accel_x 

    if (plan_X_Y_Z=='X'  ){angle  = x_val  ; }
    if (plan_X_Y_Z=='Y'  ){angle  = y_val  ; }
    if (plan_X_Y_Z=='Z'  ){angle  = z_val  ; }
    
     angle = angle + offset;
     if (angle > 360){angle = angle - 360;}

    return angle;
  }

  bool Accel::testConnection(void){
     // change de bus I2C  (TCA9648A) 
      multiplexer_i2c.select_I2C(i2c_channel); 
       return accelgyro.testConnection();
  }
 
  
  
  ;
 