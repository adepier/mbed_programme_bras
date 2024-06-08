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

int mbed_current_driven_motor::get_position() {//0 = ouvert; 1= fermé //2=non initialisé
return current_position;
} 

//********************** methodes publiques
//0 = open ,  2 = close and stop, 3 = open to endstop , 4 = close to endstop
void mbed_current_driven_motor::set_target_to_open(){             _target = 0;   } 
void mbed_current_driven_motor::set_target_to_close_and_stop(){   _target = 2;   }
void mbed_current_driven_motor::set_target_to_open_to_endstop(){  _target = 3;   }
void mbed_current_driven_motor::set_target_to_close_to_endstop(){ _target = 4;   }

void mbed_current_driven_motor::run()
{
  //0 = open ,  2 = close and stop, 3 = open to endstop , 4 = close to endstop
  if (_target == 0 && current_position != 0) {if (_debug_flag){printf("run to open\n");}            run_to_open();           current_position=0; }
  if (_target == 2 && current_position != 1) {if (_debug_flag){printf("run to close and stop\n");}  run_to_close_and_stop(); current_position=1;  }
  if (_target == 3 && current_position != 0) {if (_debug_flag){printf("run_open_to_endstop\n");}    run_open_to_endstop();   current_position=0; }
  if (_target == 4 && current_position != 1) {if (_debug_flag){printf("run to close and stop\n");}  run_close_to_endstop();  current_position=1;  }
  if (_debug_flag)  {printf("fin run moteur  \n" ); }
}

void mbed_current_driven_motor::motor_open(){
  //on ouvre
  _pwm->setPWM(_pin_IN1,0,_nominal_speed); //borneIN1 
  _pwm->setPWM(_pin_IN2,0,0); //borneIN2 
}
void mbed_current_driven_motor::motor_close(){
  //on ferme
  _pwm->setPWM(_pin_IN1,0,0); //borneIN1 
  _pwm->setPWM(_pin_IN2,0,_nominal_speed); //borneIN2 
}
void mbed_current_driven_motor::motor_stop(){
  //on stop 
    _pwm->setPWM(_pin_IN1,0,0); //borneIN1 
    _pwm->setPWM(_pin_IN2,0,0); //borneIN2
}

//********************** methodes privées

void mbed_current_driven_motor::run_to_open()
{
int i = 0;
while ( i < 10)
   { 
         motor_open();//on ouvre
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
   motor_stop();
}

void mbed_current_driven_motor::run_to_close_and_stop()
{
int i = 0;
while ( i < 10)
   {  
         motor_close(); 
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
     motor_stop();
   
}



//pour le pouce, il ya des fin de course qui coupe le courant, donc on
//fait marcher le moteur jusqu'a qu'il n'y ait plus de consommation
void mbed_current_driven_motor::run_open_to_endstop()
{
int i = 0;
//on met en route jusqu'a la butée
          motor_open();//on ouvre
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
     motor_stop();
}
//pour le pouce, il ya des fin de course qui coupe le courant, donc on
//fait marcher le moteur jusqu'a qu'il n'y ait plus de consommation
void mbed_current_driven_motor::run_close_to_endstop()
{
int i = 0;

        // //on met en route jusqu'a la butée
        motor_close();
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
     motor_stop();
}