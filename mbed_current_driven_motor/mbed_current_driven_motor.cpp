#include "mbed_current_driven_motor.h"

/*!
 *  @brief constructeur
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
mbed_current_driven_motor::mbed_current_driven_motor(INA3221 &current, 
                        mbed_PWMServoDriver &pwm,
                        int pin_IN1,
                        int pin_IN2,
                        int current_sensor_chanel,
                        string motor_name, 
                        int32_t flag_start,
                        int32_t flag_stop,
                        int current_limit,
                        int nominal_speed  )
    : _current(&current),
      _pwm(&pwm)

{


  //   default value
  _pin_IN1=pin_IN1;
  _pin_IN2=pin_IN2;
  _motor_name = motor_name;
  _current_sensor_chanel=current_sensor_chanel;
  _flag_start = flag_start;
  _flag_stop = flag_stop;
  _current_limit=current_limit;
  // définit la valeur par défaut
  _debug_flag = false;
  _nominal_speed=nominal_speed;
}



//********************** methodes publiques
void mbed_current_driven_motor::set_target_to_close_and_keep(){
  //0 = open , 1 = close and keep , 2 = close and stop, 3 = open to endstop , 4 = close to endstop
  _target = 1;
  }
void mbed_current_driven_motor::set_target_to_close_and_stop(){
  //0 = open , 1 = close and keep , 2 = close and stop, 3 = open to endstop , 4 = close to endstop
  _target = 2;
  }
  void mbed_current_driven_motor::set_target_to_open(){
   //0 = open , 1 = close and keep , 2 = close and stop, 3 = open to endstop , 4 = close to endstop
  _target = 0; 
  }

void mbed_current_driven_motor::set_target_to_open_to_endstop(){
   //0 = open , 1 = close and keep , 2 = close and stop, 3 = open to endstop , 4 = close to endstop
  _target = 3; 
  }
void mbed_current_driven_motor::set_target_to_close_to_endstop(){
   //0 = open , 1 = close and keep , 2 = close and stop, 3 = open to endstop , 4 = close to endstop
  _target = 4; 
  }

void mbed_current_driven_motor::init()
{
  // printf("init %s\n", _motor_name.c_str());
  // if (_current_limit>0)
  // {set_target_to_open();
  // }
  // else{
  //   set_target_to_open_to_endstop();
  // }
  // run(); 
  // // log
  // printf("fin init %s\n", _motor_name.c_str());
}
 

void mbed_current_driven_motor::run()
{
  //0 = open , 1 = close and keep , 2 = close and stop, 3 = open to endstop , 4 = close to endstop
  if (_target == 0)
 {
  if (_debug_flag)
      {
        printf("run to open\n");
      }
  run_to_open();
 }
  if (_target == 1)
 {
  if (_debug_flag)
      {
        printf("run to close and keep\n");
      }
  run_to_close_and_keep();
 }
  if (_target == 2)
 {
  if (_debug_flag)
      {
        printf("run to close and stop\n");
      }
  run_to_close_and_stop();
 }

  if (_target == 3)
 {
  if (_debug_flag)
      {
        printf("run_open_to_endstop\n");
      }
  run_open_to_endstop();
 }
  if (_target == 4)
 {
  if (_debug_flag)
      {
        printf("run to close and stop\n");
      }
  run_close_to_endstop();
 }
  if (_debug_flag)
  {
    printf("fin run moteur  \n" );
  }
}
//********************** methodes privées

void mbed_current_driven_motor::run_to_open()
{
int i = 0;
while ( i < 10)
   { 
         _pwm->setPWM(_pin_IN1,0,_nominal_speed); //borneIN1 
         _pwm->setPWM(_pin_IN2,0,0); //borneIN2 
        //  if faut le voir 10 fois de suite pour considérer que c'est ouvert!
         if (_current->GetCurrent(_current_sensor_chanel) * 1000 > _current_limit)
         {
           i = i + 1;
         }
         else
         {
           i = 0;
         }
   }
   //on stop 
    _pwm->setPWM(_pin_IN1,0,0); //borneIN1 
    _pwm->setPWM(_pin_IN2,0,0); //borneIN2 
}

void mbed_current_driven_motor::run_to_close_and_stop()
{
int i = 0;
while ( i < 10)
   {  
         _pwm->setPWM(_pin_IN1,0,0); //borneIN1 
         _pwm->setPWM(_pin_IN2,0,_nominal_speed); //borneIN2 
        //  if faut le voir 10 fois de suite pour considérer que c'est fermé!
         if (_current->GetCurrent(_current_sensor_chanel) * 1000 > _current_limit)
         {
           i = i + 1;
         }
         else
         {
           i = 0;
         }
   }
   //on stop 
    _pwm->setPWM(_pin_IN1,0,0); //borneIN1 
    _pwm->setPWM(_pin_IN2,0,0); //borneIN2 
   
}



void mbed_current_driven_motor::run_to_close_and_keep()
{
while ( _current->GetCurrent(_current_sensor_chanel)*1000 < _current_limit )
   {     
         _pwm->setPWM(_pin_IN1,0,0); //borneIN1 
         _pwm->setPWM(_pin_IN2,0,_nominal_speed); //borneIN2  
   }

//pour rester fermé, on diminue le pwm pour atteindre _current_limit
 int i = _nominal_speed ;
   while ( _current->GetCurrent(_current_sensor_chanel)*1000 > _current_limit )
   {     

      i= i - 100 ;
         _pwm->setPWM(_pin_IN1,0,i); //borneIN1 
         _pwm->setPWM(_pin_IN2,0,0); //borneIN2  
   }
}


void mbed_current_driven_motor::run_open_to_endstop()
{
int i = 0;
// float idle_current=0;
// //on stop 
//     _pwm->setPWM(_pin_IN1,0,0); //borneIN1 
//     _pwm->setPWM(_pin_IN2,0,0); //borneIN2 
// //on calcule le courant au repos    
// for (i = 0; i<10 ;i++){
// idle_current = idle_current + (_current->GetCurrent(_current_sensor_chanel) * 1000);
// ThisThread::sleep_for(chrono::milliseconds(20));

// }
// idle_current = (idle_current / 10) * 1.1;
// i=0;
// if (_debug_flag) { printf("idle_current %f\n",idle_current); }
//on met en route jusqu'a la butée
         _pwm->setPWM(_pin_IN1,0,_nominal_speed); //borneIN1 
         _pwm->setPWM(_pin_IN2,0,0); //borneIN2 
         ThisThread::sleep_for(chrono::milliseconds(1000));
while ( i < 10)
   { 

        //  if (_debug_flag) { printf("%f\n", (_current->GetCurrent(_current_sensor_chanel) * 1000)); }
        //  if faut le voir 10 fois de suite pour considérer que c'est ouvert!
         if ((_current->GetCurrent(_current_sensor_chanel) * 1000) < (_current_limit ))
         {
           i = i + 1;
         }
         else
         {
           i = 0;
         }
   }
   //on stop 
    _pwm->setPWM(_pin_IN1,0,0); //borneIN1 
    _pwm->setPWM(_pin_IN2,0,0); //borneIN2 
}

void mbed_current_driven_motor::run_close_to_endstop()
{
int i = 0;
// float idle_current = 0;
// //on stop 
//     _pwm->setPWM(_pin_IN1,0,0); //borneIN1 
//     _pwm->setPWM(_pin_IN2,0,0); //borneIN2 
// //on calcule le courant au repos    
// for (i = 0; i<10 ;i++){
// idle_current = idle_current + (_current->GetCurrent(_current_sensor_chanel) * 1000);
// ThisThread::sleep_for(chrono::milliseconds(20));
 
// }
// idle_current = (idle_current / 10) * 1.1;
// //on met en route jusqu'a la butée
// i=0;
// if (_debug_flag) { printf("idle_current %f\n",idle_current); }
 _pwm->setPWM(_pin_IN1,0,0); //borneIN1 
         _pwm->setPWM(_pin_IN2,0,_nominal_speed); //borneIN2 
          ThisThread::sleep_for(chrono::milliseconds(1000));
while ( i < 10)
   { 
        
        //  if (_debug_flag) { printf("%f\n", (_current->GetCurrent(_current_sensor_chanel) * 1000)); }
        //  if faut le voir 10 fois de suite pour considérer que c'est ouvert!
         if ((_current->GetCurrent(_current_sensor_chanel) * 1000) < (_current_limit ))
         {
           i = i + 1;
         }
         else
         {
           i = 0;
         }
   }
   //on stop 
    _pwm->setPWM(_pin_IN1,0,0); //borneIN1 
    _pwm->setPWM(_pin_IN2,0,0); //borneIN2 
}