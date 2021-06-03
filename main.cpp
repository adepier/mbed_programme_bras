/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 * mbed-tools compile -m NUCLEO_L432KC -t GCC_ARM -f --sterm
 * 
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
#define MAX_VAL_COUDE 7500
#define MIN_MOTOR_SPEED_COUDE 500
#define MAX_MOTOR_SPEED_COUDE 3000
//#####POIGNET
#define PIN_COUNT_POIGNET PA_8
#define PIN_STOP_POIGNET PB_1
#define PIN_DIR_POIGNET 10
#define PIN_PWM_POIGNET 11 
#define MAX_VAL_POIGNET 2500
#define MIN_MOTOR_SPEED_POIGNET 500
#define MAX_MOTOR_SPEED_POIGNET 3500

#define FLAG_START 1
#define FLAG_STOP 2
#define MOTOR_SHIELD_TYPE  1 // motor_shield_type:1=type dir/pwm -- 2=type Forward/backward

int val_coude = 0;
int val_poignet = 0;
I2C i2c(I2C_SDA, I2C_SCL);

EventFlags event_poignet;
EventFlags event_coude;

Adafruit_PWMServoDriver pwm(0x40, i2c); // Carte d'extension 16 sorties pour le pilotage de servos
                    // en PWM  (adresse I2C par defaut 0x40)
//epaule
// hall_driven_motor motor_epaule(PIN_COUNT_EPAULE, PIN_STOP_EPAULE, pwm, PIN_DIR_EPAULE, PIN_PWM_EPAULE, val1, 'A', FLAG_START_EPAULE,
//                          FLAG_STOP_EPAULE, MOTOR_SHIELD_TYPE);
//coude
hall_driven_motor motor_coude(PIN_COUNT_COUDE, PIN_STOP_COUDE, pwm, PIN_DIR_COUDE, PIN_PWM_COUDE, val_coude, 'B',   MOTOR_SHIELD_TYPE,event_coude);
//poignet
hall_driven_motor motor_poignet(PIN_COUNT_POIGNET, PIN_STOP_POIGNET, pwm, PIN_DIR_POIGNET, PIN_PWM_POIGNET, val_poignet, 'C',   MOTOR_SHIELD_TYPE,event_poignet);

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

    pwm.setPWMFreq(100); // This is the maximum PWM frequency
                       // RAZ pwm
  for (uint8_t pwmnum = 0; pwmnum < 16; pwmnum++) {
     pwm.setPWM(pwmnum, 0, 0);
  }
   printf("init coude\n");
   motor_coude.set_min_motor_speed(MIN_MOTOR_SPEED_COUDE);
  motor_coude.set_max_motor_speed(MAX_MOTOR_SPEED_COUDE); 
  motor_coude.set_coef_accel_motor(1.1);
  motor_coude.set_coef_decel_motor(5);
  motor_coude.init(); 
   printf("init poignet\n");
 motor_poignet.set_min_motor_speed(MIN_MOTOR_SPEED_POIGNET);
  motor_poignet.set_max_motor_speed(MAX_MOTOR_SPEED_POIGNET); 
  motor_poignet.set_coef_accel_motor(1.1);
  motor_poignet.set_coef_decel_motor(5);
  motor_poignet.init();



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
       
    motor->get_event_flags().wait_all(FLAG_START); // attend que le moteur ai le flag de démarrage
    printf("start motor %i",motor->get_motor_name());
    motor->run();
    motor->get_event_flags().set(FLAG_STOP); // attend que le moteur renvoie le flag de stop
     printf("stop motor %i",motor->get_motor_name());
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
  

  while (true) {
    // motor1.run(500);
    // motor2.run(500);
    // event_flags.wait_all(FLAG_STOP_EPAULE|FLAG_STOP_2 ); // attend que les moteurs
    // event_flags.wait_all(FLAG_STOP_EPAULE ); // attend que les moteurs
    val_poignet = MAX_VAL_POIGNET;
    val_coude = MAX_VAL_COUDE;
    event_coude.set(FLAG_START ); 
     
    printf("FLAG_START_COUDE %i",event_poignet.get());
    event_poignet.set(FLAG_START );

    
    //     event_flags.wait_all(FLAG_STOP_COUDE ); // attend que les moteurs
    //  event_flags.wait_all( FLAG_STOP_POIGNET); // attend que les moteurs
    //  val2 = 1800;
    //  event_flags.set(FLAG_START_2);
    //  event_flags.wait_all(FLAG_STOP_EPAULE|FLAG_STOP_2 ); // attend que les
    //  moteurs
    event_coude.wait_all(FLAG_STOP  ); // attend que les moteurs
    event_poignet.wait_all( FLAG_STOP ); // attend que les moteurs
    printf("fin1 flag %i",event_poignet.get());

    ThisThread::sleep_for(chrono::milliseconds(10000));
    
    // motor1.run(0);
    // motor2.run(0);
    val_poignet = 100;
    val_coude = 0;
    //    val2 = 0;
   event_coude.set(FLAG_START );
    //  event_flags.wait_all(FLAG_STOP_COUDE ); // attend que les moteurs
    event_poignet.set(FLAG_START );
    //  event_flags.wait_all( FLAG_STOP_POIGNET); // attend que les moteurs
    // event_flags.set(FLAG_START_2);
    //    event_flags.wait_all(FLAG_STOP_EPAULE|FLAG_STOP_2 ); // attend que les
    //    moteurs
    event_coude.wait_all(FLAG_STOP  ); // attend que les moteurs
    event_poignet.wait_all( FLAG_STOP ); // attend que les moteurs
    printf("fin2");
    ThisThread::sleep_for(chrono::milliseconds(10000));
  }
}
