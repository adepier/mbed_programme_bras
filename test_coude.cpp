/*  
  test du coude
 */

#include "config_motor.hpp"
 

//Threads moteur
Thread thread_motor_coude;

//###########################
//           INIT
//##########################
void init()
{
  printf("init PWM\n");
  pwm.begin();
  pwm.setPWMFreq(1600); // This is the maximum PWM frequency
  // RAZ pwm
  for (uint8_t pwmnum = 0; pwmnum < 16; pwmnum++)
  {
    pwm.setPWM(pwmnum, 0, 0);
  }

  // init motor
  motor_coude.init();
  // motor_coude.set_debug_flag(true);
  printf("init angle: %f\n stop 5sec...\n",motor_coude.get_angle());
  // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(5000));
}

//###########################
//           THREAD
//##########################

void run_motor_in_thread(mbed_hall_driven_motor *motor)
{
  while (true)
  {

    event_flag.wait_all(motor->_flag_start); // attend le flag de démarrage 
    // printf("start motor\n");
    //  motor->displayName();
    motor->run();
    event_flag.set(motor->_flag_stop);
    ; // attend que le moteur renvoie le flag de stop
    //  printf("stop motor %c \n",(char) motor->get_motor_name());
  }
}

//###########################
//           MAIN
//##########################
int main()
{
  // initialisation
  init();

  //démarrage des threads
  thread_motor_coude.start(callback(run_motor_in_thread, &motor_coude)); 
  //on met les moteur en place pour la premiere fois 
  //motor_coude._target = 85;//+87 deg sur le coude pour être à l'horizontal 
  printf("mise en position initiale angle: %f\n", motor_coude.get_angle());
  event_flag.set(FLAG_START_COUDE); // démarre les moteurs 
  event_flag.wait_all(FLAG_STOP_COUDE); // attend que les moteurs
  printf("fin mise en position initiale angle: %f\n stop 5sec...\n", motor_coude.get_angle());
// on attend un peu
  ThisThread::sleep_for(chrono::milliseconds(1000)); 
 
  int deplacement = 10;//120;

  while (true)
  {

    
    motor_coude._target = deplacement;//(motor_coude.get_angle() + deplacement);
    printf("start descente angle: %f\n", motor_coude.get_angle());
    event_flag.set(FLAG_START_COUDE);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_COUDE); // attend que les moteurs
    printf("fin descente angle: %f\n stop 1sec...\n", motor_coude.get_angle());
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));

    // on définit la nouvelle cible

    motor_coude._target = 0;//(motor_coude.get_angle() - deplacement);//--> point bas le moteur fait 177->89    (+87 deg sur le coude pour être à l'horizontal)

    printf("start montée angle: %f\n", motor_coude.get_angle());
    event_flag.set(FLAG_START_COUDE);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_COUDE); // attend que les moteurs
    printf("fin montée angle: %f\n stop 1sec...\n", motor_coude.get_angle());

    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}
