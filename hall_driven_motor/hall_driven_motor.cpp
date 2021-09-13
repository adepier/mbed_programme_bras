#include "hall_driven_motor.h" 

void hall_driven_motor::displayName(FirstName const& theFirstName, LastName const& theLastName)
{
  // assigning value to string s
  printf(" my name is ");
    string s = theFirstName.get();
 
    int n = s.length();
 
    // declaring character array
    char char_array[n + 1];
 
    // copying the contents of the
    // string to char array
    strcpy(char_array, s.c_str());
 
    for (int i = 0; i < n; i++)
        printf("%c",char_array[i]);
 printf(" " );
 s = theLastName.get();
 
      n = s.length();
 
    // declaring character array
      char_array[n + 1];
 
    // copying the contents of the
    // string to char array
    strcpy(char_array, s.c_str());
 
    for (int i = 0; i < n; i++)
        printf("%c",char_array[i]);
 
   printf( "\n " );
    
};
/*!
 *  @brief constructeur
 *  @param count_pin
 *  @param stop_pin
 *  @param pwm
 *  @param forward_pin
 *  @param backward_pin
 *  @param &target
 *  @param motor_name
 *  @param flag_start
 *  @param flag_stop
 *  @param
 *  @param
 *  TODO :
 *      - Ajouter un PID pour éviter
 *      - Ajouter une autocalibration pour avoir le min_motor_speed
 */
hall_driven_motor::hall_driven_motor(Count_pin count_pin, PinName stop_pin, Adafruit_PWMServoDriver &pwm, int forward_or_dir_pin, int backward_or_speed_pin, double &target_angle, double &angle, double &linked_angle, char motor_name
                                     //  , int cmde_flag_start
                                     //  , int cmde_flag_stop
                                     ,
                                     int motor_shield_type, int32_t flag_start, int32_t flag_stop, int init_speed, int min_speed, int max_speed, double coef_accel, double coef_Kp, double coef_Ki, double coef_Kd, double nb_tic_per_deg)
    : _interrupt_count(count_pin.get()), _interrupt_stop(stop_pin), _pwm(&pwm),
      _target(&target_angle), _angle(&angle), _linked_angle(&linked_angle), _PID(&Input, &Output, &Setpoint, coef_Kp, coef_Ki, coef_Kd, P_ON_E, DIRECT)

{

  // create the InterruptIn on the pin specified to Counter
  _interrupt_count.fall(callback(
      this, &hall_driven_motor::increment)); // attach increment function of
                                             // this counter instance
  _interrupt_count.rise(callback(
      this, &hall_driven_motor::increment)); // attach increment function of
                                             // this counter instance
  _interrupt_stop.fall(
      callback(this, &hall_driven_motor::stop)); // attach stop function of
                                                 // this counter instance
  _interrupt_stop.rise(
      callback(this, &hall_driven_motor::stop)); // attach stop function of
                                                 // this counter instance
  _sens = true;                                  // true = forward / false = backward
  // _flag_stop = false;
  _motor_shield_type = motor_shield_type;
  if (_motor_shield_type == 1)
  {
    _dir_pin = forward_or_dir_pin;
    _pwm_pin = backward_or_speed_pin;
  }
  if (_motor_shield_type == 2)
  {
    _forward_pin = forward_or_dir_pin;
    _backward_pin = backward_or_speed_pin;
  }
  // _min_motor_speed = 1000; // default value
  _motor_name = motor_name;
  _init_speed = init_speed;

  _min_speed = min_speed;
  _coef_accel = coef_accel;
  _coef_Kp = coef_Kp;
  _max_speed = max_speed;
  _nb_tic_per_deg = nb_tic_per_deg;

  _flag_start = flag_start;
  _flag_stop = flag_stop;
  // _cmde_flag_start = cmde_flag_start;
  // _cmde_flag_stop = cmde_flag_stop;
}

//****************** interruptions
void hall_driven_motor::increment()
{
  if (_sens)
  {
    _count++;
  }
  else
  {
    _count--;
  };
  *_angle = _count / _nb_tic_per_deg;
}
void hall_driven_motor::stop()
{
  _count = 0;
  *_angle = 0;
  _sens = true;
}

//********************** methodes publiques

void hall_driven_motor::init()
{

  //  printf("init _dir_pin: set sens=false\n");
  _interrupt_stop.enable_irq();
  _sens = false;
  // si le moteur est en butée il faut tourner dans l'autre sens
  // doucement de quelque pas pour le décoler
  while (_interrupt_stop.read() == 1)
  {
    motor_run_forward(_init_speed);
  }
  // on avance tout doucement jusqu'a 0 en mettant le sens à false
  // il deviendra true en arrivant sur stop
  while (_sens == false)
  {
    //    printf("init backward count:%i / speed:%i\n ", _count, _min_motor_speed);
    motor_run_backward(_init_speed);
  }
  //   printf(" stop\n ");
  previous_speed = 0;
  _flag_speed_sync = false;
  _interrupt_stop.disable_irq(); // --> dès qu'on arrive en butée on repart a
  motor_stop();
  _PID.SetOutputLimits(_min_speed, _max_speed);
  _PID.SetMode(1);
  _PID.SetSampleTime(1);
  printf("fin init moteur %c  \n ", _motor_name);
}

char hall_driven_motor::get_motor_name() { return _motor_name; };
int32_t hall_driven_motor::get_flag_start() { return _flag_start; };
int32_t hall_driven_motor::get_flag_stop() { return _flag_stop; };

void hall_driven_motor::set_speed_sync(bool flag)
{
  _linked_angle_offset = *_angle;
  _flag_speed_sync = flag;
  printf("linked_angle_offset:%i \n ", (int)_linked_angle_offset);
};
// EventFlags &hall_driven_motor::get_event_flags() { return *_event_flags; };

void hall_driven_motor::run()
{
  _PID.Compute(true); //restart
  previous_speed = 0;
  double target_count = *_target * _nb_tic_per_deg;
  if ((target_count - _count) > 0)
  {
    while (target_count > _count)
    {
      // calcul de la vitesse
      int speed = get_speed(target_count);
      //  printf("backward count:%i / speed:%i target_count: %i\n ",(int)  _count, speed, (int) target_count);
      _sens = true; // true = forward / false = backward
      motor_run_backward(speed);
    }
  }
  else
  {
    while (target_count < _count)
    {
      // calcul de la vitesse
      int speed = get_speed(target_count);
      //  printf("forward count:%i / speed:%i target_count: %i \n ", (int) _count, speed, (int) target_count);
      _sens = false; // true = forward / false = backward
      motor_run_forward(speed);
    }
  };
  // stop quand le compteur est arrivé
  // printf("fin target moteur %c: angle %i  \n ", _motor_name, (int)(target_count/_nb_tic_per_deg));
  motor_stop();
}
//********************** methodes privées

int hall_driven_motor::get_speed(double target)
{
  // calcul de la vitesse

  // if (*_linked_angle == *_angle)
  // {
  //   speed = int(_coef_Kp * abs(target - _count));
  //   speed = (double)min((int)speed, _max_speed); //on ne dépasse jamais le max
  //   if (previous_speed < (int)speed)
  //   { //on accélère donc on prend 100 ce qui permet de démarrer en douceur
  //     speed = (double)max((int)speed, 100);
  //     speed = (double)min((int)speed, int(previous_speed * _coef_accel));
  //   }
  //   else
  //   { //sinon on décélère donc on prend la min_speed qui permet d'atteindre l'objectif rapidement
  //     speed = (double)max((int)speed, _min_speed);
  //   }

  //   previous_speed = (int)speed;
  // }
  // else
  // {

  //on calcule l'angle a suivre :(*_linked_angle + _linked_angle_offset)
  //on calcul l'équivalent en nombre de tic : * _nb_tic_per_deg
  //on prend la différence avec le  nombre de tic  countpour avoir le target
  //pour ne pas dépasser la target, on s'arrange pour que la target soit toujours positive
  double speed_coef;
  if ((target - _count) > 0)
  {
    // Input = -(((*_linked_angle + _linked_angle_offset) * _nb_tic_per_deg) - _count);
    Input = -(target - _count);
    // calcul du coeficient de vitesse
    // on ajuste la vitesse pour que l'erreur ne depasse pas 2deg
    // dans le sens de la montée (l'angle augmente), si l'angle dépasse l'angle lié, il faut ralentir
    if (*_angle > (*_linked_angle + _linked_angle_offset) && _flag_speed_sync)
    {
      speed_coef = (1 - ((*_angle - (*_linked_angle + _linked_angle_offset)) / 1));
      if (speed_coef < 0)
      {
        speed_coef = 0;
      } // si on dépasse 2 degrès on s'arrete
    }
    else
    {
      speed_coef = 1;
    }
  }
  else
  {
    // Input = (((*_linked_angle + _linked_angle_offset) * _nb_tic_per_deg) - _count);
    Input = (target - _count);
    // calcul du coeficient de vitesse
    // on ajuste la vitesse pour que l'erreur ne depasse pas 2deg
    // dans le sens de la montée (l'angle augmente), si l'angle dépasse l'angle lié, il faut ralentir
    if (*_angle < (*_linked_angle + _linked_angle_offset) && _flag_speed_sync)
    {
      speed_coef = (1 - (((*_linked_angle + _linked_angle_offset) - *_angle) / 1));
      if (speed_coef < 0)
      {
        speed_coef = 0;
      } // si on dépasse 2 degrès on s'arrete
    }
    else
    {
      speed_coef = 1;
    }
  }

  //  {speed=   _coef_Kp*   (((*_linked_angle + _linked_angle_offset) * _nb_tic_per_deg)-_count);}
  //  else
  //  {speed= -  _coef_Kp *  (((*_linked_angle + _linked_angle_offset) * _nb_tic_per_deg)-_count);}
  Setpoint = 0; //il ne doit pas y avoir de décalage d'angle entre l'angle et le linked_angle
  _PID.Compute();
  //pour ne pas demarrer trop vite!
  previous_speed = speed;

  speed = Output * speed_coef;
  //if (speed > previous_speed + 100) {speed = previous_speed + 100;}

  // speed = (double)max((int)speed, _min_speed);
  // speed = (double)min((int)speed, _max_speed);
  // printf("%i",(int)speed);
  //  printf("moteur %c Output %i speed from pid %i /angle %i /_linked_angle %i  /Input: %i \n ", _motor_name, (int) Output, (int) speed,(int) *_angle,(int) (*_linked_angle + _linked_angle_offset) ,(int) Input);
  // }

  return (int)speed;
}
void hall_driven_motor::motor_run_forward(double speed)
{
  if (_motor_shield_type == 1)
  {
    _pwm->setPWM(_pwm_pin, 0, int(speed));
    _pwm->setPWM(_dir_pin, 0, 4095);
  }

  if (_motor_shield_type == 2)
  {
    _pwm->setPWM(_forward_pin, 0, int(speed));
    _pwm->setPWM(_backward_pin, 0, 0);
  }
}
void hall_driven_motor::motor_run_backward(double speed)
{
  if (_motor_shield_type == 1)
  {
    _pwm->setPWM(_pwm_pin, 0, int(speed));
    _pwm->setPWM(_dir_pin, 0, 0);
  }

  if (_motor_shield_type == 2)
  {
    _pwm->setPWM(_forward_pin, 0, 0);
    _pwm->setPWM(_backward_pin, 0, int(speed));
  }
}
void hall_driven_motor::motor_stop()
{
  if (_motor_shield_type == 1)
  {
    _pwm->setPWM(_pwm_pin, 0, 0);
    _pwm->setPWM(_dir_pin, 0, 0);
  }

  if (_motor_shield_type == 2)
  {
    _pwm->setPWM(_forward_pin, 0, 0);
    _pwm->setPWM(_backward_pin, 0, 0);
  }
}