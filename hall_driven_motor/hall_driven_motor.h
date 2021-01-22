#include "Adafruit_PWMServoDriver.h"
#include "mbed.h"
#include <string>

class hall_driven_motor {
public:
  hall_driven_motor(PinName count_pin,
                    PinName stop_pin,
                    Adafruit_PWMServoDriver &pwm,
                    int forward_pin,
                    int backward_pin,
                    int &cmde,
                    char name,
                    int cmde_flag_start,
                    int cmde_flag_stop);
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
  void set_motor_name(char name);
  char get_motor_name();
  int get_cmde_flag_start();
  int get_cmde_flag_stop();

private:
  InterruptIn _interrupt_count;
  InterruptIn _interrupt_stop;
  Adafruit_PWMServoDriver *_pwm;
  int _backward_pin;
  int _forward_pin;
  bool _sens;
  bool _flag_stop;
  volatile int _count;
  int _nb_count_by_turn;
  float _coef_accel_motor;
  float _coef_decel_motor;
  int _min_motor_speed;
  int speed;
  int previous_speed;
  char  _name;
  int *_cmde;
  int _cmde_flag_start;
  int _cmde_flag_stop;

};