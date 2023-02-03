/*  
  test du poignet
 */

#include "config_motor.hpp"
 

//Threads moteur
Thread thread_motor_poignet;


//###########################
//           THREAD
//##########################

void run_motor_in_thread(mbed_hall_driven_motor *motor)
{
  while (true)
  {

    event_flag.wait_all(motor->_flag_start); // attend que le moteur ai le flag de démarrage
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

 
  printf("init PWM\n");
  pwm.begin();
  pwm.setPWMFreq(1600); // This is the maximum PWM frequency
  // RAZ pwm
  for (uint8_t pwmnum = 0; pwmnum < 16; pwmnum++)
  {
    pwm.setPWM(pwmnum, 0, 0);
  }

  // init motor
  motor_poignet.init();
   printf("init angle: %f\n",  motor_poignet.get_angle());
   // on attend un peu
   // ThisThread::sleep_for(chrono::milliseconds(1000));
    // motor_poignet.set_debug_flag(true);
  
  //démarrage des threads
  thread_motor_poignet.start(callback(run_motor_in_thread, &motor_poignet));

  //on met les moteur en place pour la premiere fois 
 

  double deplacement = 5;

  while (true)
  {

    // motor_poignet._target = (motor_poignet.get_angle() + deplacement);  //--> point bas le moteur fait 89->177     (+87 deg sur le poignet pour être à l'horizontal)
    motor_poignet._target = deplacement;
    printf("start montée \n   ");
    event_flag.set(FLAG_START_POIGNET);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_POIGNET); // attend que les moteurs
    printf("fin montée angle: %f\n stop 1sec...\n",   motor_poignet.get_angle());
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));

    // on définit la nouvelle cible

    // motor_poignet._target = motor_poignet.get_angle() - deplacement; //--> point bas le moteur fait 177->89    (+87 deg sur le poignet pour être à l'horizontal)
    motor_poignet._target = 0;
    printf("start descente \n   ");
    event_flag.set(FLAG_START_POIGNET);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_POIGNET); // attend que les moteurs
    printf("fin descente angle: %f\n stop 1sec...\n",   motor_poignet.get_angle());

    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}