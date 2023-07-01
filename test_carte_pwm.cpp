/*  
  test du coude
 */

#include "config_motor.hpp"
 
 
//###########################
//           INIT
//##########################
void init()
{
  printf("init PWM\n");
  pwm.begin();
  pwm.setPWMFreq(1600); // This is the maximum PWM frequency
  // RAZ pwm
  for (uint8_t pwmnum = 0; pwmnum < 16; pwmnum++)
  {
    pwm.setPWM(pwmnum, 0, 0);
  }

  
 
}


//###########################
//           MAIN
//##########################
int main()
{
  // initialisation
  init();
  
while(true)
    {
    printf("avant2\n");
    pwm.setPWM(2, 0, 4095); //dir
    pwm.setPWM(3, 0, 4095); //pwm
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
    printf("stop\n");
    pwm.setPWM(2, 0, 0); //dir
    pwm.setPWM(3, 0, 0); //pwm
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
    printf("arriere\n");
    pwm.setPWM(2, 0, 0); //dir
    pwm.setPWM(3, 0, 4095); //pwm
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
    printf("stop\n");
    pwm.setPWM(2, 0, 0); //dir
    pwm.setPWM(3, 0, 0); //pwm
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
    }
   
}
