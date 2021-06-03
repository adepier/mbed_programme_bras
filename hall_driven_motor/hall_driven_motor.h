#include "../Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"
#include "mbed.h" 
 

class hall_driven_motor {
public:
  hall_driven_motor(PinName count_pin,
                    PinName stop_pin,
                    Adafruit_PWMServoDriver &pwm,
                    int forward_pin,
                    int backward_pin,
                    int &target,
                    char motor_name,
                    // int cmde_flag_start,
                    // int cmde_flag_stop,
                    int motor_shield_type,
                    EventFlags &event_flags
                    ,int init_speed
                    ,int min_speed
                    ,int max_speed);//motor_shield_type:1=type dir/pwm -- 2=type Forward/backward
  // interruptions
  void increment();
  void stop();
  // methodes
  void run( );
  void init();
  int read_counter();
  void set_nb_count_by_turn(int nb_count_by_turn);
  void set_coef_accel_motor(float coef_motor);
  void set_coef_decel_motor(float coef_motor);
  void set_min_motor_speed(int min_motor_speed);
  void set_max_motor_speed(int max_motor_speed);
  void set_motor_name(char motor_name);
  char get_motor_name();
  // int get_cmde_flag_start();
  // int get_cmde_flag_stop();
  EventFlags &get_event_flags();

private:
  InterruptIn _interrupt_count;
  InterruptIn _interrupt_stop;
  Adafruit_PWMServoDriver *_pwm;
  EventFlags *_event_flags;
  int _backward_pin;
  int _forward_pin;
  int _dir_pin;
  int _pwm_pin;
  bool _sens;
  bool _flag_stop;
  volatile int _count;
  int _nb_count_by_turn;
  float _coef_accel_motor;
  float _coef_decel_motor;
 int _min_speed; 
 int _max_speed;  
  int previous_speed;
  char  _motor_name;
  int *_target;
   int _init_speed;
 
  // int _cmde_flag_start;
  // int _cmde_flag_stop;
  int _motor_shield_type;
  void  motor_run_forward(int speed);
  void  motor_run_backward(int speed);
  void  motor_stop();
  int  get_speed(int target);

};