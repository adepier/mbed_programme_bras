#include "mbed_PWMServoDriver.h"
#include "mbed.h" 
#include "INA3221.h" 
/*!
 *  @brief classe pour gérer un moteur asservis par un capteur de courant  
 *  @param current_sensor 
 *  @param pwm
 *  @param pin_IN1
 *  @param pin_IN2
 *  @param current_sensor_chanel 
 *  @param motor_name 
 *  @param flag_start
 *  @param flag_stop
 *  @param current_limit 
 */
class mbed_current_driven_motor
{
public:
  mbed_current_driven_motor(INA3221 &current, 
                        mbed_PWMServoDriver &pwm,
                        int pin_IN1,
                        int pin_IN2,
                        int current_sensor_chanel,
                        string motor_name, 
                        int32_t flag_start,
                        int32_t flag_stop,
                        int current_limit,
                        int nominal_speed );

  // methodes
void run();
void init();
void set_target_to_close_and_keep();
void set_target_to_close_and_stop();
void set_target_to_open_to_endstop();
void set_target_to_close_to_endstop();
void set_target_to_open();
  
  //variables
  
  string _motor_name; 
  bool _debug_flag;
  int32_t _flag_start;
  int32_t _flag_stop;
  
private:
  int _pin_IN1;
  int _pin_IN2;
  int _current_limit;
  int _current_sensor_chanel;
  int _target; //0 = open , 1 = close and keep , 2 = close and stop
int _nominal_speed;
   INA3221 *_current;
  mbed_PWMServoDriver *_pwm;
  
  void run_to_open();
  void run_to_close_and_keep();
  void run_to_close_and_stop();
  void run_open_to_endstop();
   void run_close_to_endstop(); 
};