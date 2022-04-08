/*  
  test du coude
 */

#include "config_motor.hpp"
 

//Threads moteur
Thread thread_motor_epaule_a_plat;

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
  // motor_coude.set_debug_flag(true);
  printf("init angle: %i\n stop 5sec...\n", (int) angle_epaule_a_plat);
  // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(5000));
}

//###########################
//           THREAD
//##########################

void run_motor_in_thread(hall_driven_motor *motor)
{
  while (true)
  {

    event_flag.wait_all(motor->get_flag_start()); // attend que le moteur ai le flag de démarrage
    // printf("start motor\n");
    //  motor->displayName();
    motor->run();
    event_flag.set(motor->get_flag_stop());
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
  thread_motor_epaule_a_plat.start(callback(run_motor_in_thread, &motor_epaule_a_plat));

  //on met les moteur en place pour la premiere fois

  target_angle_epaule_a_plat = (5); //+87 deg sur le coude pour être à l'horizontal

  printf("mise en position initiale angle: %i\n", (int) angle_epaule_a_plat);
  event_flag.set(FLAG_START_EPAULE_A_PLAT); // démarre les moteurs

  event_flag.wait_all(FLAG_STOP_EPAULE_A_PLAT); // attend que les moteurs
  printf("fin mise en position initiale angle: %i\nstop 5sec...\n", (int) angle_epaule_a_plat);
// on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(5000));
  //pour afficher les courbes
  // printf("commande \t reponse \n");

  //synchronise le coude avec le poinet
  //motor_coude.set_speed_sync(angle_motor_coude,false);

  int deplacement = 45;

  while (true)
  {

    target_angle_epaule_a_plat = angle_epaule_a_plat + deplacement; //--> point bas le moteur fait 89->177     (+87 deg sur le coude pour être à l'horizontal)

    printf("start rotation horaire angle: %i\n", (int) angle_epaule_a_plat);
    event_flag.set(FLAG_START_EPAULE_A_PLAT);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_EPAULE_A_PLAT); // attend que les moteurs
    printf("fin rotation horaire angle: %i\nstop 1sec...\n", (int) angle_epaule_a_plat);
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));

    // on définit la nouvelle cible

    target_angle_epaule_a_plat = angle_epaule_a_plat - deplacement; //--> point bas le moteur fait 177->89    (+87 deg sur le coude pour être à l'horizontal)

    printf("start rotation Anti-horaire angle: %i\n", (int) angle_epaule_a_plat);
    event_flag.set(FLAG_START_EPAULE_A_PLAT);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_EPAULE_A_PLAT); // attend que les moteurs
    printf("fin rotation Anti-horaire angle: %i\nstop 1sec...\n", (int) angle_epaule_a_plat);

    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}