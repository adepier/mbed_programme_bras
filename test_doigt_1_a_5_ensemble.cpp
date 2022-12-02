/*  
  test du coude
 */

#include "config_motor.hpp"
 

//Threads moteur main
Thread thread_motor_doigt0;
Thread thread_motor_doigt1;
Thread thread_motor_doigt2;
Thread thread_motor_doigt3;
Thread thread_motor_doigt4;
Thread thread_motor_doigt5;

//###########################
//           INIT
//##########################
void init()
{
  printf("init PWM\n");
   pwm.begin();
  pwm1.begin();
   pwm.setPWMFreq(1600); // This is the maximum PWM frequency
  pwm1.setPWMFreq(1600); // This is the maximum PWM frequency
  // RAZ pwm
  for (uint8_t pwmnum = 0; pwmnum < 16; pwmnum++)
  {
   pwm.setPWM(pwmnum, 0, 0);
    pwm1.setPWM(pwmnum, 0, 0);
  } 
 //init INA3221
  current.SetAveragingMode(INA3221_AVERAGE_256);
  current.SetShuntConversionTime(INA3221_588_US);
  current.SetBusConversionTime(INA3221_140_US);

  current1.SetAveragingMode(INA3221_AVERAGE_256);
  current1.SetShuntConversionTime(INA3221_588_US);
  current1.SetBusConversionTime(INA3221_140_US);
  // init motor
  // doigt_5._debug_flag = true;
  // doigt_5.init(); 
  // on attend un peu
    // ThisThread::sleep_for(chrono::milliseconds(5000));
}

//###########################
//           THREAD
//##########################

void run_motor_doigt_in_thread(mbed_current_driven_motor *motor)
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
//           MAIN
//##########################
int main()
{
  // initialisation
  init();

  //démarrage des threads
  thread_motor_doigt0.start(callback(run_motor_doigt_in_thread, &doigt_0)); 
  thread_motor_doigt1.start(callback(run_motor_doigt_in_thread, &doigt_1)); 
  thread_motor_doigt2.start(callback(run_motor_doigt_in_thread, &doigt_2)); 
  thread_motor_doigt3.start(callback(run_motor_doigt_in_thread, &doigt_3)); 
  thread_motor_doigt4.start(callback(run_motor_doigt_in_thread, &doigt_4)); 
  thread_motor_doigt5.start(callback(run_motor_doigt_in_thread, &doigt_5)); 
  //on met les moteur en place pour la premiere fois 
  

  while (true)
  {



    doigt_1.set_target_to_open() ;// on ouvre le doigt
    doigt_2.set_target_to_open() ;// on ouvre le doigt
    doigt_3.set_target_to_open() ;// on ouvre le doigt
    doigt_4.set_target_to_open() ;// on ouvre le doigt
    doigt_5.set_target_to_open() ;// on ouvre le doigt
    printf("on ouvre le doigt \n ");
    event_flag.set(FLAG_START_DOIGT_1 | FLAG_START_DOIGT_2 | FLAG_START_DOIGT_3 | FLAG_START_DOIGT_4 | FLAG_START_DOIGT_5); // démarre les moteurs 
    event_flag.wait_all(FLAG_STOP_DOIGT_1 | FLAG_STOP_DOIGT_2 | FLAG_STOP_DOIGT_3 | FLAG_STOP_DOIGT_4 | FLAG_STOP_DOIGT_5); // attend que les moteurs
    printf("le doigt est ouvert\n stop 5sec...\n" );
         // on définit la nouvelle cible
    doigt_0.set_target_to_open_to_endstop(); //on ouvre la pince
      event_flag.set(FLAG_START_DOIGT_0); // démarre les moteurs 
    event_flag.wait_all(FLAG_STOP_DOIGT_0); // attend que les moteurs
    printf("la pince est ouverte\n  " );
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(5000)); 
    doigt_0.set_target_to_close_to_endstop(); //on ferme la pince
      event_flag.set(FLAG_START_DOIGT_0); // démarre les moteurs 
    event_flag.wait_all(FLAG_STOP_DOIGT_0); // attend que les moteurs
    printf("la pince est fermée\n  " );
     // on définit la nouvelle cible
    doigt_1.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_2.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_3.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_4.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_5.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    printf("on ferme et stop\n" );
    event_flag.set(FLAG_START_DOIGT_1 | FLAG_START_DOIGT_2 | FLAG_START_DOIGT_3 | FLAG_START_DOIGT_4 | FLAG_START_DOIGT_5);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_DOIGT_1 | FLAG_STOP_DOIGT_2 | FLAG_STOP_DOIGT_3 | FLAG_STOP_DOIGT_4 | FLAG_STOP_DOIGT_5); // attend que les moteurs
    printf("le doigt est fermé\n stop 5sec...\n" );
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(5000));

   

    // // on définit la nouvelle cible

    // doigt_5.set_target_to_close_and_keep() ;// on ouvre le doigt
    // printf("on ferme le doigt et garde fermé ");
    // event_flag.set(FLAG_START_DOIGT_5); // démarre les moteurs 
    // event_flag.wait_all(FLAG_STOP_DOIGT_5); // attend que les moteurs
    // printf("le doigt est fermé et continue de serrer pendant 5sec...\n" );
    // // on attend un peu
    // ThisThread::sleep_for(chrono::milliseconds(5000)); 
  }
}
