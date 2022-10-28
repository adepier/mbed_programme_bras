/*  
  test du coude
 */

#include "config_motor.hpp"
 

//Threads moteur
Thread thread_motor_epaule_haut;

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
  motor_epaule_haut.init();
  // motor_coude.set_debug_flag(true);
  printf("init angle: %f\n stop 5sec...\n",  motor_epaule_haut.get_angle());
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
  init();
// motor_epaule_haut.set_debug_flag(true);
  //démarrage des threads
  thread_motor_epaule_haut.start(callback(run_motor_in_thread, &motor_epaule_haut));

  //on met les moteur en place pour la premiere fois

  //motor_epaule_haut._target = (5); //+87 deg sur le coude pour être à l'horizontal

  printf("mise en position initiale angle: %f\n",    motor_epaule_haut.get_angle());
  event_flag.set(FLAG_START_EPAULE_HAUT); // démarre les moteurs 
  event_flag.wait_all(FLAG_STOP_EPAULE_HAUT); // attend que les moteurs
  printf("fin mise en position initiale angle: %f\nstop 1sec...\n",  motor_epaule_haut.get_angle());
// on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
  //pour afficher les courbes
  // printf("commande \t reponse \n");

  //synchronise le coude avec le poinet
  //motor_coude.set_speed_sync(angle_motor_coude,false);

  int deplacement = 5;

  while (true)
  {

    motor_epaule_haut._target =  motor_epaule_haut.get_angle() + deplacement; //--> point bas le moteur fait 89->177     (+87 deg sur le coude pour être à l'horizontal)

    printf("start rotation horaire angle: %f\n",  motor_epaule_haut.get_angle());
    event_flag.set(FLAG_START_EPAULE_HAUT);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_EPAULE_HAUT); // attend que les moteurs
    printf("fin rotation horaire angle: %f\nstop 1sec...\n",   motor_epaule_haut.get_angle());
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));

    // on définit la nouvelle cible

    motor_epaule_haut._target =  motor_epaule_haut.get_angle() - deplacement; //--> point bas le moteur fait 177->89    (+87 deg sur le coude pour être à l'horizontal)

    printf("start rotation Anti-horaire angle: %f\n",   motor_epaule_haut.get_angle());
    event_flag.set(FLAG_START_EPAULE_HAUT);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_EPAULE_HAUT); // attend que les moteurs
    printf("fin rotation Anti-horaire angle: %f\nstop 1sec...\n",   motor_epaule_haut.get_angle());

    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}
