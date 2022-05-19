/*  
  
 */
#include "mbed_PWMServoDriver.h"
#include "mbed_hall_driven_motor.h"
#include "mbed.h"
#include <list>

// Nucleo L432KC
#define I2C_SDA PA_10
#define I2C_SCL PA_9
//#####POIGNET_haut 
#define FLAG_START_POIGNET_HAUT  (1UL << 8) // 00000000000000000000000010000000
#define FLAG_STOP_POIGNET_HAUT (1UL << 9) // 00000000000000000000001000000000
//#####EPAULE_haut 
#define FLAG_START_EPAULE_HAUT  (1UL << 6)// 00000000000000000000000001000000
#define FLAG_STOP_EPAULE_HAUT (1UL << 7) // 00000000000000000000000010000000
//#####EPAULE_A_PLAT
#define FLAG_START_EPAULE_A_PLAT (1UL << 4) // 00000000000000000000000000010000
#define FLAG_STOP_EPAULE_A_PLAT (1UL << 5)  // 00000000000000000000000000100000
//#####COUDE 
#define FLAG_START_COUDE (1UL << 0) // 00000000000000000000000000000001
#define FLAG_STOP_COUDE (1UL << 1)  // 00000000000000000000000000000010
//#####POIGNET
#define FLAG_START_POIGNET (1UL << 2) // 00000000000000000000000000000100
#define FLAG_STOP_POIGNET (1UL << 3)  // 00000000000000000000000000001000
 

 

int flag_speed_sync_coude;

I2C i2c(I2C_SDA, I2C_SCL);

EventFlags event_flag;

mbed_PWMServoDriver pwm(0x40, i2c); // Carte d'extension 16 sorties pour le pilotage de servos
                                        // en PWM  (adresse I2C par defaut 0x40)
                                        //epaule
//epaule à plat
mbed_hall_driven_motor motor_epaule_a_plat(count_pin = PB_0,              //pin compteur de tour
                              stop_pin = PA_12,               //pin de fin de course
                              pwm,                           //carte pwm
                              forward_or_dir_pin = 12,        //pin de commande de la direction du moteur
                              backward_or_speed_pin = 13,     //pin de commande de la vitesse du moteur 
                              motor_name = "epaule a plat",          //nom du moteur
                              motor_shield_type = 1,         // motor_shield_type:1=type dir/pwm -- 2=type Forward/backward
                              flag_start = FLAG_START_EPAULE_A_PLAT, //
                              flag_stop = FLAG_STOP_EPAULE_A_PLAT,   //
                              init_speed = 1000,             //
                              min_speed = 500,                 //
                              max_speed = 4095,              //
                              coef_Kp = 0.5,                   //
                              coef_Ki = 0.005,                   //
                              coef_Kd = 0,                   //
                              nb_tic_per_deg = 43.3  ,
                                end_stop_type  = 1  
);
//epaule haut
mbed_hall_driven_motor motor_epaule_haut(count_pin = PB_5,              //pin compteur de tour
                              stop_pin = PA_11,               //pin de fin de course
                              pwm,                           //carte pwm
                              forward_or_dir_pin = 2,        //pin de commande de la direction du moteur
                              backward_or_speed_pin = 3,     //pin de commande de la vitesse du moteur 
                              motor_name = "epaule Haut",          //nom du moteur
                              motor_shield_type = 1,         // motor_shield_type:1=type dir/pwm -- 2=type Forward/backward
                              flag_start = FLAG_START_EPAULE_HAUT, //
                              flag_stop = FLAG_STOP_EPAULE_HAUT,   //
                              init_speed = 1500,             //
                              min_speed = 500,                 //
                              max_speed = 4095,              //
                              coef_Kp = 0.5,                   //
                              coef_Ki = 0.005,                   //
                              coef_Kd = 0,                   //
                              nb_tic_per_deg = 59  ,
                                end_stop_type  = 1  
);
//coude
mbed_hall_driven_motor motor_coude(count_pin = PB_6,              //pin compteur de tour
                              stop_pin = PB_7,               //pin de fin de course
                              pwm,                           //carte pwm
                              forward_or_dir_pin = 0,        //pin de commande de la direction du moteur
                              backward_or_speed_pin = 1,     //pin de commande de la vitesse du moteur 
                              motor_name = "Coude",          //nom du moteur
                              motor_shield_type = 1,         // motor_shield_type:1=type dir/pwm -- 2=type Forward/backward
                              flag_start = FLAG_START_COUDE, //
                              flag_stop = FLAG_STOP_COUDE,   //
                              init_speed = 1000,             //
                              min_speed = 500,                 //
                              max_speed = 4095,              //
                              coef_Kp = 1,                   //
                              coef_Ki = 0.005,                   //
                              coef_Kd = 0,                   //
                              nb_tic_per_deg = 43.3  ,
                                end_stop_type  = 1  
);
//poignet
mbed_hall_driven_motor motor_poignet(count_pin = PA_8,
                                stop_pin = PB_1,
                                pwm,
                                forward_or_dir_pin = 10,
                                backward_or_speed_pin = 11, 
                                motor_name = "Poignet",
                                motor_shield_type = 1,
                                flag_start = FLAG_START_POIGNET,
                                flag_stop = FLAG_STOP_POIGNET,
                                init_speed = 3500,
                                min_speed =500,
                                max_speed = 4095,
                                coef_Kp = 1,  //
                                coef_Ki = 0.01, //
                                coef_Kd = 0,  //
                                nb_tic_per_deg = 38.8 ,
                                end_stop_type  = 1  );

//poignet haut
DigitalOut pinPB4(PB_4,0);
DigitalOut pinPA2(PA_2,0);
mbed_hall_driven_motor motor_poignet_haut(count_pin = PB_4 ,
                                stop_pin = PA_3  ,
                                pwm,
                                forward_or_dir_pin = 6,
                                backward_or_speed_pin = 7, 
                                motor_name = "Poignet_haut",
                                motor_shield_type = 1, // comme le type 1 mais inversé
                                flag_start = FLAG_START_POIGNET_HAUT,
                                flag_stop = FLAG_STOP_POIGNET_HAUT,
                                init_speed = 3500,
                                min_speed =1000,
                                max_speed = 4095,
                                coef_Kp = 1,  //
                                coef_Ki = 0.01, //
                                coef_Kd = 0,  //
                                nb_tic_per_deg = 38.8 ,
                                end_stop_type  = 0);

