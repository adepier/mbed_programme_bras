#include "hall_driven_motor.h"
 
/*!
 *  @brief constructeur
 *  @param count_pin
 *  @param stop_pin
 *  @param pwm
 *  @param forward_pin
 *  @param backward_pin
 *  @param &cmde
 *  @param name
 *  @param flag_start
 *  @param flag_stop
 *  @param
 *  @param
 *  TODO :
 *      - Ajouter un PID pour éviter
 *      - Ajouter une autocalibration pour avoir le min_motor_speed
 */
hall_driven_motor::hall_driven_motor(PinName count_pin, PinName stop_pin,
                                     Adafruit_PWMServoDriver &pwm,
                                     int forward_pin, int backward_pin,
                                     int &cmde, char name, int cmde_flag_start,
                                     int cmde_flag_stop, int motor_shield_type,
                                     EventFlags &event_flags)
    : _interrupt_count(count_pin), _interrupt_stop(stop_pin), _pwm(&pwm),
      _cmde(&cmde),_event_flags(&event_flags )  { // create the InterruptIn on the pin specified to Counter
  _interrupt_count.fall(callback(
      this, &hall_driven_motor::increment)); // attach increment function of
                                             // this counter instance
  _interrupt_count.rise(callback(
      this, &hall_driven_motor::increment)); // attach increment function of
                                             // this counter instance
  _interrupt_stop.fall(
      callback(this, &hall_driven_motor::stop)); // attach stop function of
                                                 // this counter instance
  _sens = true; // true = forward / false = backward
  _flag_stop = false;
  _motor_shield_type = motor_shield_type;
  if (_motor_shield_type == 1) {
    _dir_pin = forward_pin;
    _pwm_pin = backward_pin;
  }
  if (_motor_shield_type == 2) {
    _forward_pin = forward_pin;
    _backward_pin = backward_pin;
  }
  _min_motor_speed = 1000; // default value
  _name = name;
  _cmde_flag_start = cmde_flag_start;
  _cmde_flag_stop = cmde_flag_stop;
  
}

//****************** interruptions
void hall_driven_motor::increment() {
  if (_sens) {
    _count++;
  } else {
    _count--;
  };
}
void hall_driven_motor::stop() {
  _count = 0;
  _sens = true;
}

//********************** methodes publiques

void hall_driven_motor::run() { 
  if ((*_cmde - _count) > 0) { 
    while (*_cmde > _count) {
      // calcul de la vitesse
      int speed = get_speed(*_cmde);
    //   printf("backward count:%i / speed:%i\n ", _count, speed);
      _sens = true; // true = forward / false = backward
      motor_run_backward(speed); 
    }
  } else {
    while (*_cmde < _count) {
        // calcul de la vitesse
      int speed = get_speed(*_cmde);
    //   printf("forward count:%i / speed:%i\n ", _count, speed);
      _sens = false; // true = forward / false = backward
      motor_run_forward(speed);
    }
  };
   // stop quand le compteur est arrivé
  printf("%c: count %i  \n ", _name, _count);
  motor_stop();
}

void hall_driven_motor::init() {

  //  printf("init _dir_pin: set sens=false\n");
  _interrupt_stop.enable_irq();
  _sens = false; 
  // si le moteur est en butée il faut tourner dans l'autre sens
  // doucement de quelque pas pour le décoler
  while (_interrupt_stop.read() == 1) {
    motor_run_forward(_min_motor_speed);
  }
  // on avance tout doucement jusqu'a 0 en mettant le sens à false
  // il deviendra true en arrivant sur stop
  while (_sens == false) {
//    printf("init backward count:%i / speed:%i\n ", _count, _min_motor_speed);
    motor_run_backward(_min_motor_speed);
  }
  //   printf(" stop\n ");
  previous_speed = _min_motor_speed;
  _interrupt_stop.disable_irq();
  motor_stop();
  printf("fin init moteur %c  \n ", _name);
}

int hall_driven_motor::read_counter() { return _count; }
void hall_driven_motor::set_nb_count_by_turn(int nb_count_by_turn) {
  _nb_count_by_turn = nb_count_by_turn;
}
void hall_driven_motor::set_coef_accel_motor(float coef_accel_motor) {
  _coef_accel_motor = coef_accel_motor;
}
void hall_driven_motor::set_coef_decel_motor(float coef_decel_motor) {
  _coef_decel_motor = coef_decel_motor;
}
void hall_driven_motor::set_min_motor_speed(int min_motor_speed) {
  _min_motor_speed = min_motor_speed;
}
void hall_driven_motor::set_max_motor_speed(int max_motor_speed) {
  _max_motor_speed = max_motor_speed;
}

void hall_driven_motor::set_motor_name(char name) { _name = name; };

char hall_driven_motor::get_motor_name() { return _name; };
int hall_driven_motor::get_cmde_flag_start() { return _cmde_flag_start; };
int hall_driven_motor::get_cmde_flag_stop() { return _cmde_flag_stop; };
EventFlags &hall_driven_motor::get_event_flags() { return *_event_flags; };


//********************** methodes privées

int  hall_driven_motor::get_speed(int target){
    // calcul de la vitesse
    int speed;
      speed = int(_coef_decel_motor * abs(target - _count));
      speed = max(speed, _min_motor_speed);
      speed = min(speed, _max_motor_speed);
      speed = min(speed, int(previous_speed * _coef_accel_motor));
      previous_speed = speed; 
      return speed;
}
void hall_driven_motor::motor_run_forward(int speed) {
  if (_motor_shield_type == 1) {
    _pwm->setPWM(_pwm_pin,0, speed) ;
    _pwm->setPWM(_dir_pin,0 ,4095);
  }

  if (_motor_shield_type == 2) {
    _pwm->setPWM(_forward_pin, 0, speed);
    _pwm->setPWM(_backward_pin, 0, 0);
  }
}
void hall_driven_motor::motor_run_backward(int speed) {
  if (_motor_shield_type == 1) {
    _pwm->setPWM(_pwm_pin, 0, speed);
    _pwm->setPWM(_dir_pin, 0, 0);
  }

  if (_motor_shield_type == 2) {
    _pwm->setPWM(_forward_pin, 0, 0);
    _pwm->setPWM(_backward_pin, 0, speed);
  }
}
void hall_driven_motor::motor_stop() {
  if (_motor_shield_type == 1) {
    _pwm->setPWM(_pwm_pin, 0, 0);
    _pwm->setPWM(_dir_pin, 0, 0);
  }

  if (_motor_shield_type == 2) {
    _pwm->setPWM(_forward_pin, 0, 0);
    _pwm->setPWM(_backward_pin, 0, 0);
  }
}