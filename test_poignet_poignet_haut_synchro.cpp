#include "config_motor.hpp"


//Threads moteur
Thread thread_motor_poignet(osPriorityNormal,1000, nullptr,  nullptr);
Thread thread_motor_poignet_haut(osPriorityNormal,1000, nullptr,  nullptr);

//###########################
//           THREAD
//##########################

void run_motor_in_thread(mbed_hall_driven_motor *motor)
{
  while (true)
  { 
    
    event_flag.wait_all(motor->_flag_start); // attend le flag de démarrage 
    motor->run();
    event_flag.set(motor->_flag_stop);
     
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
  
  // init moteur
  motor_poignet_haut.init();
  motor_poignet.init();

//démarrage des moteurs
  thread_motor_poignet.start(callback(run_motor_in_thread, &motor_poignet));
  thread_motor_poignet_haut.start(callback(run_motor_in_thread, &motor_poignet_haut));

  //synchronise les moteurs
    motor_poignet.set_speed_sync(&motor_poignet_haut); 
    motor_poignet_haut.set_speed_sync(&motor_poignet); 
 
  // motor_epaule_haut.set_debug_flag(true);
  //  motor_coude.set_debug_flag(true);
}


//###########################
//           MAIN
//##########################
int main()
{
  // initialisation
  init();
 
  int deplacement =190; 

  while (true)
  {
     motor_poignet._target = deplacement; // définit les target
     motor_poignet_haut._target = deplacement; // définit les target
    

    printf("start deplacement \n");
    event_flag.set( FLAG_START_POIGNET|FLAG_START_POIGNET_HAUT);    // démarre les moteurs
    event_flag.wait_all( FLAG_STOP_POIGNET|FLAG_STOP_POIGNET_HAUT); // attend que les moteurs
    printf("fin deplacement \n");
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));

    // on définit la nouvelle cible

    motor_poignet._target = 0; // définit les target
     motor_poignet_haut._target = 0; // définit les target
    
    printf("start retour a zero \n");
    event_flag.set( FLAG_START_POIGNET|FLAG_START_POIGNET_HAUT);    // démarre les moteurs
    event_flag.wait_all( FLAG_STOP_POIGNET|FLAG_STOP_POIGNET_HAUT); // attend que les moteurs
    printf("fin retour a zero \n");

    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}
