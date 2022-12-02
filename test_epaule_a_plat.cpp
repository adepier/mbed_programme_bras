/*  
  test du coude
 */

#include "config_motor.hpp"
 

//Threads moteur
Thread thread_motor_epaule_a_plat;

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
  motor_epaule_a_plat.init();
  //démarrage des threads
  thread_motor_epaule_a_plat.start(callback(run_motor_in_thread, &motor_epaule_a_plat));
 // motor_coude.set_debug_flag(true);
  printf("init angle: %f\n ", motor_epaule_a_plat.get_angle());
 
}


//###########################
//           MAIN
//##########################
int main()
{
  // initialisation
  init();


  int deplacement = 180;

  while (true)
  {

    motor_epaule_a_plat._target =  deplacement; //--> point bas le moteur fait 89->177     (+87 deg sur le coude pour être à l'horizontal)

    printf("start rotation horaire angle: %f\n", motor_epaule_a_plat.get_angle());
    event_flag.set(FLAG_START_EPAULE_A_PLAT);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_EPAULE_A_PLAT); // attend que les moteurs
    printf("fin rotation horaire angle: %f\nstop 1sec...\n", motor_epaule_a_plat.get_angle());
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));

    // on définit la nouvelle cible

    motor_epaule_a_plat._target =0; //--> point bas le moteur fait 177->89    (+87 deg sur le coude pour être à l'horizontal)

    printf("start rotation Anti-horaire angle: %f\n", motor_epaule_a_plat.get_angle());
    event_flag.set(FLAG_START_EPAULE_A_PLAT);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_EPAULE_A_PLAT); // attend que les moteurs
    printf("fin rotation Anti-horaire angle: %f\nstop 1sec...\n", motor_epaule_a_plat.get_angle());

    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}
