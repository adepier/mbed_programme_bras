/*  
  test du coude
 */

#include "config_motor.hpp"
 

//Threads moteur
Thread thread_motor_epaule_haut;
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
//           INIT
//##########################
void init()
{
  
  // char data[] = {PCA9685_MODE1, MODE1_RESTART};
  //   if(i2c.write(0x40, data, 2))
  //   {  
  //       printf(" No ACK on i2c write!\n"); 
  //   }
  printf("init PWM\n");
  pwm.begin();
    printf("setPWMFreq PWM\n");
  pwm.setPWMFreq(1600); // This is the maximum PWM frequency
  // RAZ pwm
  for (uint8_t pwmnum = 0; pwmnum < 16; pwmnum++)
  {
    pwm.setPWM(pwmnum, 0, 0);
  }

  // init motor
          printf("Count2 :   %d:   %d:   %d\n", encoder2.count,encoder2.tic_backward,encoder2.tic_forward);
  motor_epaule_haut.init();
  // motor_coude.set_debug_flag(true);
 //démarrage des threads
  thread_motor_epaule_haut.start(callback(run_motor_in_thread, &motor_epaule_haut));
 printf("init angle: %f\n stop 5sec...\n",  motor_epaule_haut.get_angle());
          printf("Count2 :   %d:   %d:   %d\n", encoder2.count,encoder2.tic_backward,encoder2.tic_forward);
}


//###########################
//           MAIN
//##########################
int main()
{
  // initialisation
  init();

  int deplacement = 5;

  while (true)
  {

    motor_epaule_haut._target =   deplacement; //--> point bas le moteur fait 89->177     (+87 deg sur le coude pour être à l'horizontal)

    printf("start rotation horaire angle: %f\n",  motor_epaule_haut.get_angle());
    event_flag.set(FLAG_START_EPAULE_HAUT);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_EPAULE_HAUT); // attend que les moteurs
    printf("fin rotation horaire angle: %f\nstop 1sec...\n",   motor_epaule_haut.get_angle());
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1s));

    // on définit la nouvelle cible

    motor_epaule_haut._target = 0; //--> point bas le moteur fait 177->89    (+87 deg sur le coude pour être à l'horizontal)

    printf("start rotation Anti-horaire angle: %f\n",   motor_epaule_haut.get_angle());
    event_flag.set(FLAG_START_EPAULE_HAUT);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_EPAULE_HAUT); // attend que les moteurs
    printf("fin rotation Anti-horaire angle: %f\nstop 1sec...\n",   motor_epaule_haut.get_angle());

    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1s));
  }
}
