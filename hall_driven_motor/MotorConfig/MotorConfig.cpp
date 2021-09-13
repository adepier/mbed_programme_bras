
#include "MotorConfig.h" 

MotorConfig::MotorConfig(PinName count_pin,     //////////////////////
                PinName stop_pin,      //////////////////////
                int forward_or_dir_pin,       ///////////////////////////////////////////
                int backward_or_speed_pin,      /////////////////////////////////////////
                char motor_name,       ///////////////
                int motor_shield_type, /////////////////////////////////////////////////////////////
                int32_t flag_start,    //////////////////////
                int32_t flag_stop,     /////////////////////
                int init_speed,        ///////////////////////////////
                int min_speed,         //////////////
                int max_speed,         ////////////// 
                double coef_Kp,        ////////////////////////
                double coef_Ki,        ///////////////////
                double coef_Kd,        //////////////////
                double nb_tic_per_deg   ) 
{
     _count_pin = count_pin;
      _stop_pin = stop_pin;
      _forward_or_dir_pin =forward_or_dir_pin;
      _backward_or_speed_pin=backward_or_speed_pin;
      _motor_name=motor_name;
      _motor_shield_type=motor_shield_type;
      _flag_start=flag_start;
      _flag_stop=flag_stop;
      _init_speed=init_speed;
      _min_speed=min_speed;
      _max_speed=max_speed; 
      _coef_Kp=coef_Kp;
      _coef_Ki=coef_Ki;
      _coef_Kd=coef_Kd;
      _nb_tic_per_deg=nb_tic_per_deg;
    
}

