#include "hall_driven_motor.h"

void hall_driven_motor::displayName()
{
  // assigning value to string s
  string s = get_motor_name();
  int n = s.length();
  // declaring character array
  char char_array[n + 1];

  // copying the contents of the
  // string to char array
  strcpy(char_array, s.c_str());
  for (int i = 0; i < n; i++)
    printf("%c", char_array[i]);
  printf("\n");
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
hall_driven_motor::hall_driven_motor(Count_pin count_pin,
                                     Stop_pin stop_pin,
                                     Adafruit_PWMServoDriver &pwm,
                                     Forward_or_dir_pin forward_or_dir_pin,
                                     Backward_or_speed_pin backward_or_speed_pin,
                                     double &target_angle,
                                     double &angle,
                                     Motor_name motor_name,
                                     Motor_shield_type motor_shield_type,
                                     Flag_start flag_start,
                                     Flag_stop flag_stop,
                                     Init_speed init_speed,
                                     Min_speed min_speed,
                                     Max_speed max_speed,
                                     Coef_Kp coef_Kp,
                                     Coef_Ki coef_Ki,
                                     Coef_Kd coef_Kd,
                                     Nb_tic_per_deg nb_tic_per_deg)
    : _interrupt_count(count_pin.get()),
      _interrupt_stop(stop_pin.get()),
      _pwm(&pwm),
      _target(&target_angle),
      _angle(&angle),
      _PID(&Input, &Output, &Setpoint, coef_Kp.get(), coef_Ki.get(), coef_Kd.get(), P_ON_E, DIRECT)

{

  // create the InterruptIn on the pin specified to Counter
  _interrupt_count.fall(callback(
      this, &hall_driven_motor::increment)); // attach increment function of
                                             // this counter instance
  _interrupt_count.rise(callback(
      this, &hall_driven_motor::increment)); // attach increment function of
                                             // this counter instance
  // _interrupt_stop.fall(
  //     callback(this, &hall_driven_motor::stop)); // attach stop function of
  //                                                // this counter instance
  _interrupt_stop.rise(
      callback(this, &hall_driven_motor::stop)); // attach stop function of
                                                 // this counter instance
  _sens = true;                                  // true = forward / false = backward
  // _flag_stop = false;
  _motor_shield_type = motor_shield_type.get();
  if (_motor_shield_type == 1)
  {
    _dir_pin = forward_or_dir_pin.get();
    _pwm_pin = backward_or_speed_pin.get();
  }
  if (_motor_shield_type == 2)
  {
    _forward_pin = forward_or_dir_pin.get();
    _backward_pin = backward_or_speed_pin.get();
  }
  // _min_motor_speed = 1000; // default value
  _motor_name = motor_name.get();
  _init_speed = init_speed.get();

  _min_speed = min_speed.get();
  _coef_Kp = coef_Kp.get();
  _max_speed = max_speed.get();
  _nb_tic_per_deg = nb_tic_per_deg.get();

  _flag_start = flag_start.get();
  _flag_stop = flag_stop.get();

  //définit la valeur par défaut
  _flag_speed_sync = false;
  _debug_flag = false;
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
  printf("init ");
  displayName();

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
  // _PID.SetSampleTime(1);
  printf("fin init ");
  displayName();
}

string hall_driven_motor::get_motor_name() { return _motor_name; };
int32_t hall_driven_motor::get_flag_start() { return _flag_start; };
int32_t hall_driven_motor::get_flag_stop() { return _flag_stop; };

void hall_driven_motor::set_speed_sync(double &linked_angle, bool flag)
{
  //on récupère l'écart d'angle au moment de la synchro

  if (flag == true)
  {

    _linked_angle = &linked_angle;
    //on doit avoir (*_angle+ _linked_angle_offset) = (*_linked_angle )
    //donc
    _linked_angle_offset = *_linked_angle -*_angle;
    _flag_speed_sync = flag;
    printf("linked_angle_offset:%i \n ", (int)_linked_angle_offset);
  }
  else
  {
    _flag_speed_sync = flag;
  }
};

//on met le flag debug pour afficher les données de debug
void hall_driven_motor::set_debug_flag(bool flag) { _debug_flag = flag; };
void hall_driven_motor::run()
{
  _PID.Compute(true); //restart
  previous_speed = 0;

  double target_count = *_target * _nb_tic_per_deg;

  if ((target_count - _count) > 0)
  {
    //on recule
    while (target_count > _count)
    {
      // calcul de la vitesse
      int speed = get_speed(target_count);
      if (_debug_flag)
      {
        printf("backward count:%i / speed:%i target_count: %i\n ", (int)_count, speed, (int)target_count);
      }
      _sens = true; // true = forward / false = backward
      motor_run_backward(speed);
    }
  }
  else
  {
    //on avance
    while (target_count < _count)
    {
      // calcul de la vitesse
      int speed = get_speed(target_count);
      if (_debug_flag)
      {
        printf("forward count:%i / speed:%i target_count: %i \n ", (int)_count, speed, (int)target_count);
      }
      _sens = false; // true = forward / false = backward
      motor_run_forward(speed);
    }
  };
  // stop quand le compteur est arrivé
  if (_debug_flag)
  {
    printf("fin target moteur : angle %i  \n ", (int)(target_count / _nb_tic_per_deg));
  }
  motor_stop();
}
//********************** methodes privées

int hall_driven_motor::get_speed(double target)
{

  // calcul de la vitesse avec le PID
  Setpoint = 0; //il ne doit pas y avoir de décalage d'angle entre l'angle et le linked_angle
  Input = -abs(target - _count);

  //le PID demarre à 1500 de la target.
  if (abs(target - _count) < 1500)
  {
    _PID.Compute(); //le PID calcule la vitesse et commence a prendre en compte les valeurs pour l'intégrale
    speed = Output;
  }
  else
  {
    _PID.Compute(true); //on fait calculer le PID en reinitialisant la période de temp
    speed = Output;     //le PID calcule la vitesse et sans a prendre en compte le temp pour l'intégrale
  }

  //calcul du coef de vitesse pour la synchro

  //on calcule l'angle a suivre :(*_linked_angle + _linked_angle_offset)
  //on calcul l'équivalent en nombre de tic : * _nb_tic_per_deg
  //on prend la différence avec le  nombre de tic  countpour avoir le target
  //pour ne pas dépasser la target, on s'arrange pour que la target soit toujours positive

  // calcul du coeficient de vitesse
  // on ajuste la vitesse pour que l'erreur ne depasse pas 2deg
  double speed_coef;
  double Nb_deg_autorise = 0.1;
  if (_flag_speed_sync)
  {
    if (target > _count)
    {//backward
      // quand l'angle augmente, si l'angle est plus grand que l'angle lié, il faut ralentir
      if ((*_angle+ _linked_angle_offset) > (*_linked_angle ))
      {
        // coef = pourcentage de l'erreur d'angle 
        //          2 deg -> 0%
        //          1 deg -> 50%
        //          0 deg -> 100%
        // coef = (Nb_deg_autorisé - erreur d'angle) / Nb_deg_autorisé
        // à 2 degrès on stop le moteur
        speed_coef = (Nb_deg_autorise - abs(*_angle - (*_linked_angle + _linked_angle_offset)) / Nb_deg_autorise);
        if (speed_coef < 0) // entre 0 et 2 degres on ralentit
        {
          speed_coef = 0;
        } // si on dépasse 2 degrès on s'arrete
      }
      else
      {
        speed_coef = 1;
      }
    }
    if (target < _count) {
      //forward
      // quand l'angle diminue, si l'angle est plus petit que l'angle lié, il faut ralentir
      if ((*_angle+ _linked_angle_offset) < (*_linked_angle ) )
      {
        speed_coef = (Nb_deg_autorise - abs(*_angle - (*_linked_angle + _linked_angle_offset)) / Nb_deg_autorise);
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
  }
  else
  {
    speed_coef = 1;
  }
  
   //on applique le coeficient pour la synchro avec la vitesse
  speed = speed * speed_coef;
  
  
  //debug
  if (_debug_flag)
  {
    printf(" get_speed target %i", (int)target);
    printf(" Input %i ", (int)Input);
    printf(" speed %i ", (int)speed);
    // speed coef
    printf(" _count %i ", (int)_count);
    printf(" angle %i ", (int)*_angle);
    printf(" speed_coef %i ", (int)speed_coef);
    printf(" linked_angle %i ", (int)*_linked_angle);
    printf(" _linked_angle_offset %i ", (int)_linked_angle_offset);
  }

 
  //pour ne pas trembler, la vitesse ne peut pas être en dessous de la vitesse mini
  //if (speed <_min_speed) {speed=_min_speed;}
  //pour ne pas demarrer trop vite, on n'augmente pas la vitesse de plus de 100 par cycle

  if (speed > previous_speed + 100)
  {
    speed = previous_speed + 100;
  }
  previous_speed = speed;
 


  // speed = (double)max((int)speed, _min_speed);
  // speed = (double)min((int)speed, _max_speed);
  // printf("%i",(int)speed);
  // }

  return (int)speed;
}
void hall_driven_motor::motor_run_forward(double speed)
{
  // la vitesse max est 4095
  if (speed > 4095)
  {
    speed = 4095;
  }

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
  // la vitesse max est 4095
  if (speed > 4095)
  {
    speed = 4095;
  }
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