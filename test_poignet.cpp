/*  
  test du poignet
 */

#include "config_motor.hpp"
 

//Threads moteur
Thread thread_motor_poignet;

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
  motor_poignet.init();
   printf("init angle: %i\n", (int) angle_poignet);
   // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(10000));
    // motor_poignet.set_debug_flag(true);
}

//###########################
//           THREAD
//##########################

void run_motor_in_thread(mbed_hall_driven_motor *motor)
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
  thread_motor_poignet.start(callback(run_motor_in_thread, &motor_poignet));

  //on met les moteur en place pour la premiere fois

  target_angle_poignet = (10); //+87 deg sur le poignet pour être à l'horizontal

  printf("mise en position initiale angle: %i\n", (int) angle_poignet);
  event_flag.set(FLAG_START_POIGNET); // démarre les moteurs

  event_flag.wait_all(FLAG_STOP_POIGNET); // attend que les moteurs
  printf("fin mise en position initiale angle: %i\n stop 5sec...\n", (int) angle_poignet);

// on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(5000));

  //pour afficher les courbes
  // printf("commande \t reponse \n");

  //synchronise le poignet avec le poinet
  //motor_poignet.set_speed_sync(angle_motor_poignet,false);

  int deplacement = 110;

  while (true)
  {

    target_angle_poignet = angle_poignet + deplacement; //--> point bas le moteur fait 89->177     (+87 deg sur le poignet pour être à l'horizontal)

    printf("start montée \n   ");
    event_flag.set(FLAG_START_POIGNET);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_POIGNET); // attend que les moteurs
    printf("fin montée angle: %i\n stop 1sec...\n", (int) angle_poignet);
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));

    // on définit la nouvelle cible

    target_angle_poignet = angle_poignet - deplacement; //--> point bas le moteur fait 177->89    (+87 deg sur le poignet pour être à l'horizontal)

    printf("start descente \n   ");
    event_flag.set(FLAG_START_POIGNET);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_POIGNET); // attend que les moteurs
    printf("fin descente angle: %i\n stop 1sec...\n", (int) angle_poignet);

    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}
