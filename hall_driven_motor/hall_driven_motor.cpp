#include "hall_driven_motor.h"

/*!
 *  @brief constructeur
 *  @param count_pin
 *  @param stop_pin
 *  @param pwm
 *  @param forward_pin
 *  @param backward_pin
 *  @param &target
 *  @param &angle
 *  @param motor_name
 *  @param Motor_shield_type
 *  @param flag_start
 *  @param flag_stop
 *  @param Init_speed
 *  @param
  
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
  //   default value
  _motor_name = motor_name.get();
  _init_speed = init_speed.get();
  _min_speed = min_speed.get();
  _max_speed = max_speed.get();
  _nb_tic_per_deg = nb_tic_per_deg.get();

  _flag_start = flag_start.get();
  _flag_stop = flag_stop.get();

  //définit la valeur par défaut
  _flag_speed_sync = false;
  _debug_flag = false;
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
  //met à jour l'angle du moteur
  *_angle = _count / _nb_tic_per_deg;
}

//********************** methodes publiques

void hall_driven_motor::init()
{
  printf("init ");
  displayName();

  _interrupt_stop.enable_irq(); // --> on allume la lecture de la butée
  // _interrupt_stop.read() == 1 --> en butée
  //on fait tourner le moteur jusqu'a la butée
  while (_interrupt_stop.read() == 1)
  {
    motor_run_forward(_init_speed);
  }
  motor_stop();                                      //on arrete le moteur
  ThisThread::sleep_for(chrono::milliseconds(1000)); //on attend une seconde pour stabiliser le moteur

  //on repart tout doucement pour que l'init soit juste après la butée
  while (_interrupt_stop.read() == 0)
  {
    motor_run_backward(_init_speed / 2);
  }
  motor_stop();                  //on arrete le moteur
  _interrupt_stop.disable_irq(); // --> on eteint la lecture de la butée

  //on initialise les variables
  previous_speed = 0;
  _flag_speed_sync = false;
  _count = 0;
  *_angle = 0;
  _sens = true;
  _PID.SetOutputLimits(_min_speed, _max_speed);
  _PID.SetMode(1);

  // log
  printf("fin init ");
  displayName();
}

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
    _linked_angle_offset = *_linked_angle - *_angle;
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

  previous_speed = 0;

  double target_count = *_target * _nb_tic_per_deg; //calcul la target en nombre de tic

  if ((target_count - _count) > 0)
  {
    //on recule
    while (target_count > _count)
    {
      // calcul de la vitesse à chaque tour
      int speed = get_speed(target_count);
      if (_debug_flag)
      {
        printf("   backward\n");
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
      // calcul de la vitesse à chaque tour
      int speed = get_speed(target_count);
      if (_debug_flag)
      {
        printf("   forward\n");
      }
      _sens = false; // true = forward / false = backward
      motor_run_forward(speed);
    }
  };
  // stop quand le compteur est arrivé
  if (_debug_flag)
  {
    printf("fin target moteur : angle %i  \n", (int)(target_count / _nb_tic_per_deg));
  }
  motor_stop();
}
//********************** methodes privées
void hall_driven_motor::displayName()
{
  //pour le debug, affiche le nom du moteur

  char char_array[_motor_name.length() + 1]; // declaring character array
  strcpy(char_array, _motor_name.c_str());   // copying the contents of the string to char array
  //print chaque lettre
  for (int i = 0; i < _motor_name.length(); i++)
    printf("%c", char_array[i]);
  printf("\n");
};

int hall_driven_motor::get_speed(double target)
{

  // calcul de la vitesse avec le PID
  Setpoint = 0; //il ne doit pas y avoir de décalage d'angle entre l'angle et le linked_angle
  Input = -abs(target - _count);
  double speed;
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

  // calcul du coeficient de vitesse pour la synchro
  // on ajuste la vitesse pour que l'erreur ne depasse pas 2deg
  // coef = pourcentage de l'erreur d'angle
  //          2 deg -> 0%
  //          1 deg -> 50%
  //          0 deg -> 100%
  // coef = (Nb_deg_autorisé - erreur d'angle) / Nb_deg_autorisé
  // à 2 degrès on stop le moteur
  double speed_coef;
  double Nb_deg_autorise = 1;

  if (_flag_speed_sync)
  {
    speed_coef = (Nb_deg_autorise - abs((*_angle + _linked_angle_offset) - (*_linked_angle))) / Nb_deg_autorise;
    // le coeficient ne peut pas etre negatif, cela veut dire que l'on a dépassé l'angle autorisé, donc on s'arrete
    if (speed_coef < 0)
    {
      speed_coef = 0;
    }

    //backward => quand l'angle augmente et que l'angle est plus petit que l'angle lié, on est en retard -> donc pas de speed_coef
    if (target > _count && ((*_angle + _linked_angle_offset) < (*_linked_angle)))
    {
      speed_coef = 1;
    }
    //forward => quand l'angle diminue, si l'angle est plus grand que l'angle lié, on est en retard -> donc pas de speed_coef
    if (target < _count && ((*_angle + _linked_angle_offset) > (*_linked_angle)))
    {
      speed_coef = 1;
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
    printf("get_speed target %i\t", (int)(target * 1000));
    // printf("\t Input %i ", (int)Input);//-> = -count
    printf("speed %i\t", (int)(speed * 1000));
    printf("count %i\t", (int)(_count * 1000));
    printf("angle %i\t", (int)(*_angle * 1000));
    printf("speed_coef %i\t", (int)(speed_coef * 1000));
    printf("linked_angle %i\t", (int)(*_linked_angle * 1000));
    printf("linked_angle_offset %i\t", (int)(_linked_angle_offset * 1000));
  }

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