#include "config_motor.hpp"

//Threads moteur
Thread thread_motor_coude;
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

  // init poignet
  motor_poignet.init();

  // init coude
  motor_coude.init();

  // motor_poignet.set_debug_flag(true);
  //  motor_coude.set_debug_flag(true);
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
  thread_motor_coude.start(callback(run_motor_in_thread, &motor_coude));
  thread_motor_poignet.start(callback(run_motor_in_thread, &motor_poignet));

  //on met les moteur en place pour la premiere fois
  target_angle_poignet = 5;      //point haut poignet
  target_angle_coude = (5 + 87); //+87 deg sur le coude pour être à l'horizontal

  printf("mise en position initiale \n");
  event_flag.set(FLAG_START_POIGNET | FLAG_START_COUDE);    // démarre les moteurs
  event_flag.wait_all(FLAG_STOP_POIGNET | FLAG_STOP_COUDE); // attend que les moteurs
  printf("fin mise en position initiale \n");

  //pour afficher les courbes
  // printf("commande \t reponse \n");

  //synchronise le coude avec le poinet
  // motor_coude.set_speed_sync(angle_poignet,true);
  //   motor_poignet.set_speed_sync(angle_coude,true);

  int deplacement = 90;

  while (true)
  {
    target_angle_poignet = angle_poignet + deplacement; //--> point bas le moteur fait 2->92
    target_angle_coude = angle_coude + deplacement;     //--> point bas le moteur fait 89->177     (+87 deg sur le coude pour être à l'horizontal)

    printf("start descente \n   ");
    event_flag.set(FLAG_START_POIGNET | FLAG_START_COUDE);    // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_POIGNET | FLAG_STOP_COUDE); // attend que les moteurs
    printf("fin descente \n   ");
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));

    // on définit la nouvelle cible

    target_angle_poignet = angle_poignet - deplacement; //--> point haut le moteur fait 92->2
    target_angle_coude = angle_coude - deplacement;     //--> point bas le moteur fait 177->89    (+87 deg sur le coude pour être à l'horizontal)

    printf("start montée \n   ");
    event_flag.set(FLAG_START_POIGNET | FLAG_START_COUDE);    // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_POIGNET | FLAG_STOP_COUDE); // attend que les moteurs
    printf("fin montée \n");

    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}
