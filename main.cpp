#include "config_motor.hpp"

//Threads moteur
Thread thread_motor_coude;
Thread thread_motor_poignet;
Thread thread_motor_epaule_a_plat;
Thread thread_motor_epaule_haut;

//###########################
//           THREAD
//##########################

void run_motor_in_thread(hall_driven_motor *motor)
{
  while (true)
  {
    event_flag.wait_all(motor->_flag_start); // attend le flag de démarrage 
    motor->run();
    event_flag.set(motor->_flag_stop);
     
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
  for (int pwmnum = 0; pwmnum < 16; pwmnum++)
  {
    pwm.setPWM(pwmnum, 0, 0);
  }

  // init epaule_a_plat
  motor_epaule_a_plat.init();
  // init poignet
  motor_poignet.init();
  // init coude
  motor_coude.init();
  // init epaule_haut
  motor_epaule_haut.init();

  // motor_poignet.set_debug_flag(true);
  //  motor_epaule_a_plat.set_debug_flag(true);
  //motor_coude._debug_flag=true;
  
 

  //on met les moteur en place pour la premiere fois
  motor_poignet._target = 90;      //point haut poignet
  motor_coude._target = (5 + 87); //+87 deg sur le coude pour être à l'horizontal
  motor_epaule_a_plat._target = 5;
  motor_epaule_haut._target = 5;

  printf("mise en position initiale \n");
   //démarrage des moteurs
  thread_motor_coude.start(callback(run_motor_in_thread, &motor_coude));
  thread_motor_poignet.start(callback(run_motor_in_thread, &motor_poignet));
  thread_motor_epaule_a_plat.start(callback(run_motor_in_thread, &motor_epaule_a_plat));
  thread_motor_epaule_haut.start(callback(run_motor_in_thread, &motor_epaule_haut));
   
  event_flag.set(FLAG_START_POIGNET | FLAG_START_COUDE | FLAG_START_EPAULE_A_PLAT | FLAG_START_EPAULE_HAUT);  // démarre les moteurs 
  event_flag.wait_all(FLAG_STOP_POIGNET | FLAG_STOP_COUDE | FLAG_STOP_EPAULE_A_PLAT | FLAG_STOP_EPAULE_HAUT ); // attend que les moteurs
  // on attend un peu
  ThisThread::sleep_for(chrono::milliseconds(1000));
  printf("fin mise en position initiale \n");
   
  //pour afficher les courbes
  // printf("commande \t reponse \n");
  //pour snchroniser les moteurs, il faut que chaque moteur connaisse l'angle et le deplacement
  //des autres moteurs
  // on fait une donc une collection avec ces infos, chaque moteur va avoir accès a cette collection
  // pour la synchro on va donc iterer sur chaque moteur pour savoir si on est en retard ou en avance
  // hall_driven_motor *synchronised_motor_list[3] = {&motor_coude, &motor_epaule_a_plat, &motor_epaule_haut};

  // hall_driven_motor* synchronised_motor_list =  {&motor_coude  } ;
  //synchronise les moteurs
    motor_coude.set_speed_sync(&motor_epaule_a_plat);
    motor_coude.set_speed_sync(&motor_epaule_haut);
    motor_coude.set_speed_sync(&motor_poignet);

    motor_epaule_haut.set_speed_sync(&motor_epaule_a_plat);
    motor_epaule_haut.set_speed_sync(&motor_coude);
    motor_epaule_haut.set_speed_sync(&motor_poignet);

    motor_epaule_a_plat.set_speed_sync(&motor_coude);
    motor_epaule_a_plat.set_speed_sync(&motor_epaule_haut);
    motor_epaule_a_plat.set_speed_sync(&motor_poignet);

    motor_poignet.set_speed_sync(&motor_coude);
    motor_poignet.set_speed_sync(&motor_epaule_haut);
    motor_poignet.set_speed_sync(&motor_epaule_a_plat);   
  // motor_coude.set_speed_sync_2(angle_epaule_a_plat,true,true);

  double deplacement_poignet = 90;
  double deplacement_coude = 45;
  double deplacement_epaule_haut = 45;
  double deplacement_epaule_a_plat = 20;

  while (true)
  {
    motor_poignet._target = (motor_poignet._angle + deplacement_poignet); //--> point bas le moteur fait 2->92
    motor_coude._target = (motor_coude._angle + deplacement_coude);       //--> point bas le moteur fait 89->177     (+87 deg sur le coude pour être à l'horizontal)
    motor_epaule_a_plat._target = (motor_epaule_a_plat._angle + deplacement_epaule_a_plat);
    motor_epaule_haut._target = (motor_epaule_haut._angle + deplacement_epaule_haut);
    
    // motor_coude._debug_flag=true;
    // motor_epaule_haut._debug_flag=true; 
    printf("start descente \n");
     //démarrage des moteurs 
    event_flag.set(FLAG_START_COUDE |FLAG_START_POIGNET |  FLAG_START_EPAULE_A_PLAT | FLAG_START_EPAULE_HAUT);  // démarre les moteurs 
    event_flag.wait_all(FLAG_STOP_POIGNET | FLAG_STOP_COUDE | FLAG_STOP_EPAULE_A_PLAT | FLAG_STOP_EPAULE_HAUT); // attend que les moteurs 
    printf("fin descente \n");
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));

    // on définit la nouvelle cible
    motor_poignet._target = (motor_poignet._angle - deplacement_poignet); //--> point bas le moteur fait 92->2
    motor_coude._target = (motor_coude._angle - deplacement_coude);       //--> point bas le moteur fait 177->89     (+87 deg sur le coude pour être à l'horizontal)
    motor_epaule_a_plat._target = (motor_epaule_a_plat._angle - deplacement_epaule_a_plat);
    motor_epaule_haut._target = (motor_epaule_haut._angle - deplacement_epaule_haut);

    printf("start montée \n");
     
    event_flag.set(FLAG_START_COUDE |FLAG_START_POIGNET |  FLAG_START_EPAULE_A_PLAT | FLAG_START_EPAULE_HAUT);  // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_POIGNET | FLAG_STOP_COUDE | FLAG_STOP_EPAULE_A_PLAT | FLAG_STOP_EPAULE_HAUT); // attend que les moteurs
    printf("fin montée \n");

    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}
