/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 * mbed-tools compile -m NUCLEO_L432KC   -t GCC_ARM --mbed-os-path "C:\Users\adepier\Google Drive\Antoine\projets C++\MBED\mbed-os" -f --sterm
 * mbed-tools compile -m NUCLEO_L432KC   -t GCC_ARM --mbed-os-path "C:\Users\adepier\Google Drive\Antoine\projets C++\MBED\mbed-os" --clean
 * mbed-tools configure -m NUCLEO_L432KC -t GCC_ARM --mbed-os-path "C:\Users\adepier\Google Drive\Antoine\projets C++\MBED\mbed-os"
 */
#include "adafruit_pwmservodriver/Adafruit_PWMServoDriver.h"
#include "hall_driven_motor/hall_driven_motor.h"
#include "mbed.h"
#include <cstdint>

// Nucleo L432KC
#define I2C_SDA PA_10
#define I2C_SCL PA_9
//#####EPAULE
#define PIN_COUNT_EPAULE PB_0
#define PIN_STOP_EPAULE PA_12
#define PIN_DIR_EPAULE 12
#define PIN_PWM_EPAULE 13
#define FLAG_START_EPAULE 0x1
#define FLAG_STOP_EPAULE 0x2
#define MAX_VAL_EPAULE 2500
#define MIN_MOTOR_SPEED_EPAULE 200
#define MAX_MOTOR_SPEED_EPAULE 1000
//#####EPAULE_haut - MPU6050
#define PIN_DIR_EPAULE_HAUT 2
#define PIN_PWM_EPAULE_HAUT 3
#define FLAG_START_EPAULE_HAUT 0x3
#define FLAG_STOP_EPAULE_HAUT 0x4
#define MAX_VAL_EPAULE_HAUT 2500
#define MIN_MOTOR_SPEED_EPAULE_HAUT 200
#define MAX_MOTOR_SPEED_EPAULE_HAUT 1000
//#####COUDE
#define PIN_COUNT_COUDE PB_6
#define PIN_STOP_COUDE PB_7
#define PIN_DIR_COUDE 0
#define PIN_PWM_COUDE 1 
#define NB_TIC_PER_DEG_COUDE 43.3 
#define INIT_SPEED_COUDE 1500
#define MIN_MOTOR_SPEED_COUDE 0
#define MAX_MOTOR_SPEED_COUDE 4500
#define COEF_ACCEL_COUDE 0 //inutile
#define COEF_KP_COUDE 1
#define COEF_KI_COUDE 1
#define COEF_KD_COUDE 0
#define FLAG_START_COUDE (1UL << 0) // 00000000000000000000000000000001
#define FLAG_STOP_COUDE (1UL << 1) // 00000000000000000000000000000010
//#####POIGNET
#define PIN_COUNT_POIGNET PA_8
#define PIN_STOP_POIGNET PB_1
#define PIN_DIR_POIGNET 10
#define PIN_PWM_POIGNET 11 
#define NB_TIC_PER_DEG_POIGNET 38.8 
#define INIT_SPEED_POIGNET 2500
#define MIN_MOTOR_SPEED_POIGNET 0
#define MAX_MOTOR_SPEED_POIGNET 4500
#define COEF_ACCEL_POIGNET 1.01
#define COEF_KP_POIGNET 1
#define COEF_KI_POIGNET 15
#define COEF_KD_POIGNET 0
#define FLAG_START_POIGNET (1UL << 2) // 00000000000000000000000000000100
#define FLAG_STOP_POIGNET (1UL << 3)  // 00000000000000000000000000001000

#define FLAGS_START (FLAG_START_POIGNET | FLAG_START_COUDE)   // this is a bit-wise OR (not a logical OR)
#define FLAGS_STOP (FLAG_STOP_POIGNET | FLAG_STOP_COUDE)   // this is a bit-wise OR (not a logical OR)

#define FLAG_START 1
#define FLAG_STOP 2
#define MOTOR_SHIELD_TYPE  1 // motor_shield_type:1=type dir/pwm -- 2=type Forward/backward

double target_angle_coude = 0;
double angle_coude;
double angle_poignet;
double linked_angle_coude;
int flag_speed_sync_coude;
double target_angle_poignet = 0;
I2C i2c(I2C_SDA, I2C_SCL);

EventFlags event_flag;

Adafruit_PWMServoDriver pwm(0x40, i2c); // Carte d'extension 16 sorties pour le pilotage de servos
                    // en PWM  (adresse I2C par defaut 0x40)
//epaule
// hall_driven_motor motor_epaule(PIN_COUNT_EPAULE, PIN_STOP_EPAULE, pwm, PIN_DIR_EPAULE, PIN_PWM_EPAULE, val1, 'A', FLAG_START_EPAULE,
//                          FLAG_STOP_EPAULE, MOTOR_SHIELD_TYPE);
//coude
hall_driven_motor motor_coude(PIN_COUNT_COUDE
                              , PIN_STOP_COUDE
                              , pwm, PIN_DIR_COUDE
                              , PIN_PWM_COUDE
                              , target_angle_coude //target
                              , angle_coude //adresse du compteur, il faut en faire une variable gloable pour l'utiliser dans les autres moteurs
                              , angle_poignet //linked count :adresse du compteur lié
                              , 'C'
                              , MOTOR_SHIELD_TYPE
                              , FLAG_START_COUDE
                              , FLAG_STOP_COUDE
                              , INIT_SPEED_COUDE 
                              , MIN_MOTOR_SPEED_COUDE 
                              , MAX_MOTOR_SPEED_COUDE
                              ,COEF_ACCEL_COUDE
                              ,COEF_KP_COUDE
                              ,COEF_KI_COUDE
                              ,COEF_KD_COUDE
                              ,NB_TIC_PER_DEG_COUDE  );
//poignet
hall_driven_motor motor_poignet(PIN_COUNT_POIGNET
                              , PIN_STOP_POIGNET
                              , pwm
                              , PIN_DIR_POIGNET
                              , PIN_PWM_POIGNET
                              , target_angle_poignet
                              , angle_poignet
                              , angle_coude
                              , 'P'
                              , MOTOR_SHIELD_TYPE
                              , FLAG_START_POIGNET
                              , FLAG_STOP_POIGNET
                              , INIT_SPEED_POIGNET 
                              , MIN_MOTOR_SPEED_POIGNET 
                              , MAX_MOTOR_SPEED_POIGNET
                               ,COEF_ACCEL_POIGNET
                              ,COEF_KP_POIGNET
                              ,COEF_KI_POIGNET
                              ,COEF_KD_POIGNET
                              , NB_TIC_PER_DEG_POIGNET );

// hall_driven_motor motor2(PB_1,PA_8, pwm,7,
// 6,val2,'B',FLAG_START_2,FLAG_STOP_2,MOTOR_SHIELD_TYPE);
Thread thread_motor_coude;
Thread thread_motor_poignet;



//###########################
//           INIT
//##########################
void init() {
  printf("init\n");

  pwm.begin();

    pwm.setPWMFreq(1600); // This is the maximum PWM frequency
                       // RAZ pwm
  for (uint8_t pwmnum = 0; pwmnum < 16; pwmnum++) {
     pwm.setPWM(pwmnum, 0, 0);
  }
  
   printf("init poignet\n");
//  motor_poignet.set_min_motor_speed(MIN_MOTOR_SPEED_POIGNET);
//   motor_poignet.set_max_motor_speed(MAX_MOTOR_SPEED_POIGNET); 
  // motor_poignet.set_coef_accel_motor(1.05);
  // motor_poignet.set_coef_decel_motor(5);
  motor_poignet.init();

 printf("init coude\n");
  //  motor_coude.set_min_motor_speed(MIN_MOTOR_SPEED_COUDE);
  // motor_coude.set_max_motor_speed(MAX_MOTOR_SPEED_COUDE); 
  // motor_coude.set_coef_accel_motor(1.1);
  // motor_coude.set_coef_decel_motor(5);
  motor_coude.init(); 

  // motor2.set_min_motor_speed(1000);
  // motor2.set_coef_accel_motor(1.1);
  // motor2.set_coef_decel_motor(5);
  // motor2.init();
}


//###########################
//           THREAD
//##########################

void run_motor_in_thread(hall_driven_motor *motor) {
 
  while (true) {
       
    event_flag.wait_all(motor->get_flag_start()); // attend que le moteur ai le flag de démarrage
    // printf("start motor %c \n",(char) motor->get_motor_name());
    motor->run();
    event_flag.set(motor->get_flag_stop());; // attend que le moteur renvoie le flag de stop
    //  printf("stop motor %c \n",(char) motor->get_motor_name());
  }
}



//###########################
//           MAIN
//##########################
int main() {

  init();

//   int flag1 = 1;
//   int flag2 = 2;
//   uint32_t flags_read = 0;

  thread_motor_coude.start(callback(run_motor_in_thread, &motor_coude));
 
  thread_motor_poignet.start(callback(run_motor_in_thread, &motor_poignet ));
  
//on met les moteur en place pour la premiere fois
target_angle_poignet = int(2  );//point haut poignet
 target_angle_coude = int((2 + 87)  ) ;//+87 deg sur le coude pour être à l'horizontal
  event_flag.set(FLAGS_START) ;  
event_flag.wait_all(FLAGS_STOP) ; // attend que les moteurs 
printf("commande \t reponse \n");
//demarreg la synchro des moteurs
motor_coude.set_speed_sync(true);
motor_poignet.set_speed_sync(false);

  while (true) {
    // motor1.run(500);
    // motor2.run(500);
    // event_flags.wait_all(FLAG_STOP_EPAULE|FLAG_STOP_2 ); // attend que les moteurs
    // event_flags.wait_all(FLAG_STOP_EPAULE ); // attend que les moteurs
    int deplacement  = 90;
    int angle_poignet_init = int(2 );//point haut poignet
    int angle_coude_init = int(89 );//point haut poignet
    target_angle_poignet = angle_poignet_init + deplacement ; //--> point bas le moteur fait 2->92
    target_angle_coude = angle_coude_init + deplacement   ;//--> point bas le moteur fait 89->177     (+87 deg sur le coude pour être à l'horizontal)

    
    // printf("start descente S-curve \n" );
   
  
    ///S-curve croissante
    // for(int i =1; i <= deplacement+1; i++) {
    //     //  linked_angle_coude = angle_coude_init + ( deplacement *  ( (1.0 / ( 1.0 + exp(double(-i))))));
    //     //  linked_angle_poignet =angle_poignet_init + ( deplacement *  ( (1.0 / ( 1.0 + exp(double(-i))))));
    //      linked_angle_coude = angle_coude_init + i; //lineaire
    //      linked_angle_poignet =angle_poignet_init + i;
    // linked_angle_coude = target_angle_coude;
    // linked_angle_poignet = target_angle_poignet;
    event_flag.set(FLAGS_START) ;  
      // for(int i =-1000; i <= 1000; i++) {
      //    linked_angle_coude = (double)angle_coude_init + ( (double)deplacement *  (((double)1.0 / ((double)1 + exp(double(-i*0.008))))));
      //    linked_angle_poignet =(double)angle_poignet_init + ( (double)deplacement *  ( ((double)1.0 / ((double)1 + exp(double(-i*0.008))))));
      // printf("descente %i angle_coude %i / %i / %i \t linked_angle_poignet %i / %i / %i \n",i,(int) linked_angle_coude,(int) angle_coude,(int) target_angle_coude,(int ) linked_angle_poignet,(int) angle_poignet,(int) target_angle_poignet);
      //     if (i%10==0) printf(" %i \t %i \n", (int) linked_angle_coude,(int) angle_coude);
      //     ThisThread::sleep_for(chrono::milliseconds((2)));
         
      //     event_flag.set(FLAGS_START) ;  
           
      //   // ThisThread::sleep_for(chrono::milliseconds(1000)         );
      //      }
    event_flag.wait_all(FLAGS_STOP) ; // attend que les moteurs 

    
    
    //     event_flags.wait_all(FLAG_STOP_COUDE ); // attend que les moteurs
    //  event_flags.wait_all( FLAG_STOP_POIGNET); // attend que les moteurs
    //  val2 = 1800;
    //  event_flags.set(FLAG_START_2);
    //  event_flags.wait_all(FLAG_STOP_EPAULE|FLAG_STOP_2 ); // attend que les
    //  moteurs
    
      printf("fin descente \n   " );

    ThisThread::sleep_for(chrono::milliseconds(1000));
    
    // motor1.run(0);
    // motor2.run(0);
     
     
    // printf("start remontée " );
    //    val2 = 0;
     angle_poignet_init = angle_poignet;//point haut poignet
      angle_coude_init = angle_coude;//point haut poignet
  target_angle_poignet = angle_poignet_init - deplacement ; //--> point haut le moteur fait 92->2
  target_angle_coude = angle_coude_init - deplacement   ;//--> point bas le moteur fait 177->89    (+87 deg sur le coude pour être à l'horizontal)
    
    // linked_angle_coude = target_angle_coude;
    // linked_angle_poignet = target_angle_poignet;
    event_flag.set(FLAGS_START) ;  

    // for(int i =-1000; i <= 1000; i++) {
    //      linked_angle_coude = (double)angle_coude_init - ( (double)deplacement *  (((double)1.0 / ((double)1 + exp(double(-i*0.01))))));
    //      linked_angle_poignet =(double)angle_poignet_init - ( (double)deplacement *  ( ((double)1.0 / ((double)1 + exp(double(-i*0.01))))));
    //     for(int i =0; i <= 1000; i++) {
       
    //      linked_angle_coude = (double)angle_coude_init - ((double)deplacement * (double)i/(double)1000.0); //lineaire
    //      linked_angle_poignet = (double)angle_poignet_init - ((double)deplacement * (double)i/(double)1000.0);
    //               //  printf("montee linked_angle_coude %i / %i / %i  \t linked_angle_poignet %i / %i / %i  \n",(int) linked_angle_coude,(int) angle_coude,(int) target_angle_coude,(int ) linked_angle_poignet,(int) angle_poignet,(int) target_angle_poignet);
    //      if (i%10==0) printf(" %i \t %i \n", (int) linked_angle_coude,(int) angle_coude);
    // //       event_coude.set(FLAG_START);
    // // //  event_flags.wait_all(FLAG_STOP_COUDE ); // attend que les moteurs
    // // event_poignet.set(FLAG_START );
    // //     event_coude.wait_all(FLAG_STOP  ); // attend que les moteurs
    // // event_poignet.wait_all( FLAG_STOP ); // attend que les moteurs
    //        event_flag.set(FLAGS_START) ;  
    //      ThisThread::sleep_for(chrono::milliseconds(2));
       
    //     //  ThisThread::sleep_for(chrono::milliseconds(1000));
    //        }
   event_flag.wait_all(FLAGS_STOP) ; // attend que les moteurs 
    //  event_flags.wait_all( FLAG_STOP_POIGNET); // attend que les moteurs
    // event_flags.set(FLAG_START_2);
    //    event_flags.wait_all(FLAG_STOP_EPAULE|FLAG_STOP_2 ); // attend que les
    //    moteurs

      printf("fin montée \n" );
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}
