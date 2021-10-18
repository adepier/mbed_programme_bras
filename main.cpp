#include "config_motor.hpp"

//Threads moteur
Thread thread_motor_coude;
Thread thread_motor_poignet;
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
  
 // init epaule_a_plat
  motor_epaule_a_plat.init();
  // init poignet
  motor_poignet.init();

  // init coude
  motor_coude.init();

 

  // motor_poignet.set_debug_flag(true);
  //  motor_epaule_a_plat.set_debug_flag(true);
   motor_coude.set_debug_flag(true);
}

//###########################
//           THREAD
//##########################

void run_motor_in_thread(hall_driven_motor *motor)
{
  while (true)
  { 
    
    event_flag.wait_all(motor->get_flag_start()); // attend que le moteur ai le flag de démarrage 
    motor->run();
    event_flag.set(motor->get_flag_stop());
     
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
  thread_motor_epaule_a_plat.start(callback(run_motor_in_thread, &motor_epaule_a_plat));

  //on met les moteur en place pour la premiere fois
   motor_poignet.set_target(5)  ;      //point haut poignet
   motor_coude.set_target(5 + 87); //+87 deg sur le coude pour être à l'horizontal
   motor_epaule_a_plat.set_target(5);

  printf("mise en position initiale \n");
  event_flag.set(FLAG_START_POIGNET | FLAG_START_COUDE|FLAG_START_EPAULE_A_PLAT);    // démarre les moteurs
  event_flag.wait_all(FLAG_STOP_POIGNET | FLAG_STOP_COUDE|FLAG_STOP_EPAULE_A_PLAT); // attend que les moteurs
  printf("fin mise en position initiale \n");

  //pour afficher les courbes
  // printf("commande \t reponse \n");
//pour snchroniser les moteurs, il faut que chaque moteur connaisse l'angle et le deplacement
//des autres moteurs
// on fait une donc une collection avec ces infos, chaque moteur va avoir accès a cette collection
// pour la synchro on va donc iterer sur chaque moteur pour savoir si on est en retard ou en avance
    hall_driven_motor* synchronised_motor_list[3]  = {&motor_coude,&motor_epaule_a_plat,&motor_poignet  } ;
    
   // hall_driven_motor* synchronised_motor_list =  {&motor_coude  } ;



  //synchronise le coude avec le poinet
      motor_coude.set_speed_sync ( synchronised_motor_list ,3,true);
      motor_poignet.set_speed_sync ( synchronised_motor_list,3,true);
       motor_epaule_a_plat.set_speed_sync ( synchronised_motor_list,3,true);
  // motor_coude.set_speed_sync_2(angle_epaule_a_plat,true,true);

  // motor_poignet.set_speed_sync_1(angle_coude,true,true);

  // motor_epaule_a_plat.set_speed_sync_1(angle_coude,true,true);

// motor_coude.set_debug_flag(true);

  double deplacement = 90;
  double deplacement_epaule_a_plat = 20;

  while (true)
  {
    motor_poignet.set_target ( motor_poignet._angle + deplacement); //--> point bas le moteur fait 2->92
    motor_coude.set_target  (motor_coude._angle + deplacement);     //--> point bas le moteur fait 89->177     (+87 deg sur le coude pour être à l'horizontal)
    motor_epaule_a_plat.set_target ( motor_epaule_a_plat._angle + deplacement_epaule_a_plat); 

    printf("start descente \n");
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
    printf("start descente 1 \n");
    event_flag.set(FLAG_START_POIGNET | FLAG_START_COUDE|FLAG_START_EPAULE_A_PLAT);    // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_POIGNET | FLAG_STOP_COUDE|FLAG_STOP_EPAULE_A_PLAT); // attend que les moteurs
    printf("fin descente \n");
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));

    // on définit la nouvelle cible
    motor_poignet.set_target ( motor_poignet.get_angle() - deplacement); //--> point bas le moteur fait 92->2
    motor_coude.set_target  (motor_coude.get_angle() - deplacement);     //--> point bas le moteur fait 177->89     (+87 deg sur le coude pour être à l'horizontal)
    motor_epaule_a_plat.set_target ( motor_epaule_a_plat.get_angle() - deplacement_epaule_a_plat); 

    printf("start montée \n");
    event_flag.set(FLAG_START_POIGNET | FLAG_START_COUDE|FLAG_START_EPAULE_A_PLAT);    // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_POIGNET | FLAG_STOP_COUDE|FLAG_STOP_EPAULE_A_PLAT); // attend que les moteurs
    printf("fin montée \n");

    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}
