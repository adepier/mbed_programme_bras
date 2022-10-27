/*  
  
 */
#include "mbed_PWMServoDriver.h"
#include "mbed_hall_driven_motor.h"
#include "mbed.h"
#include "mbed_current_driven_motor.h"
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
 //#####DOIGT 5 (petit doigt)
#define FLAG_START_DOIGT_5  (1UL << 10) // 00000000000000000000000010000000
#define FLAG_STOP_DOIGT_5 (1UL << 11) // 00000000000000000000001000000000
//#####DOIGT 4 (annulaire)
#define FLAG_START_DOIGT_4  (1UL << 12) // 00000000000000000000000010000000
#define FLAG_STOP_DOIGT_4 (1UL << 13) // 00000000000000000000001000000000
//#####DOIGT 3 (majeur)
#define FLAG_START_DOIGT_3  (1UL << 14) // 00000000000000000000000010000000
#define FLAG_STOP_DOIGT_3 (1UL << 15) // 00000000000000000000001000000000
//#####DOIGT 2 (index)
#define FLAG_START_DOIGT_2  (1UL << 16) // 00000000000000000000000010000000
#define FLAG_STOP_DOIGT_2 (1UL << 17) // 00000000000000000000001000000000
//#####DOIGT 1 (pouce)
#define FLAG_START_DOIGT_1  (1UL << 18) // 00000000000000000000000010000000
#define FLAG_STOP_DOIGT_1 (1UL << 19) // 00000000000000000000001000000000
//#####DOIGT 0 (pouce bas)
#define FLAG_START_DOIGT_0  (1UL << 20) // 00000000000000000000000010000000
#define FLAG_STOP_DOIGT_0 (1UL << 21) // 00000000000000000000001000000000

int flag_speed_sync_coude;

I2C i2c(I2C_SDA, I2C_SCL);

EventFlags event_flag;

mbed_PWMServoDriver pwm(0x40, i2c); // Carte d'extension 16 sorties pour le pilotage de servos
                                        // en PWM  (adresse I2C par defaut 0x40)
                                        //epaule
mbed_PWMServoDriver pwm1(0x41, i2c);  // deuxieme carte d'extension 16 sorties pour le pilotage de servos                                       
INA3221 current(i2c,0x42);
INA3221 current1(i2c,0x43);

/****************************   BRAS  *******************************************************************************/

Encoder encoder1;
Encoder encoder2;
Encoder encoder3;
Encoder encoder4;
Encoder encoder5;

Pin_interrupt counter1_1(D2,1,encoder1 );
Pin_interrupt counter1_2(D3,2,encoder1 );
Pin_interrupt counter2_1(D4,1,encoder2 );
Pin_interrupt counter2_2(D5,2,encoder2 );
Pin_interrupt counter3_1(D6,1,encoder3 );
Pin_interrupt counter3_2(D9,2,encoder3 );
Pin_interrupt counter4_1(D10,1,encoder4 );
Pin_interrupt counter4_2(D11,2,encoder4 );
Pin_interrupt counter5_1(D12,1,encoder5 );
Pin_interrupt counter5_2(D13,2,encoder5 );

//epaule à plat
mbed_hall_driven_motor motor_epaule_a_plat(encoder1.count ,                           //count_2_pin 
                              stop_pin = A0,               //pin de fin de course
                              pwm,                            //carte pwm
                              forward_or_dir_pin = 12,        //pin de commande de la direction du moteur
                              backward_or_speed_pin = 13,     //pin de commande de la vitesse du moteur 
                              motor_name = "epaule a plat",   //nom du moteur
                              motor_shield_type = 1,          // motor_shield_type:1=type dir/pwm -- 2=type Forward/backward
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
mbed_hall_driven_motor motor_epaule_haut(encoder2.count ,                        //count_2_pin
                              stop_pin = A1,               //pin de fin de course
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
mbed_hall_driven_motor motor_coude( encoder3.count ,                        //count_2_pin
                              stop_pin = A2,               //pin de fin de course
                              pwm,                           //carte pwm
                              forward_or_dir_pin = 0,        //pin de commande de la direction du moteur
                              backward_or_speed_pin = 1,     //pin de commande de la vitesse du moteur 
                              motor_name = "Coude",          //nom du moteur
                              motor_shield_type = 1,         // motor_shield_type:1=type dir/pwm -- 2=type Forward/backward
                              flag_start = FLAG_START_COUDE, //
                              flag_stop = FLAG_STOP_COUDE,   //
                              init_speed = 2500,             //
                              min_speed = 500,                 //
                              max_speed = 4095,              //
                              coef_Kp = 1,                   //
                              coef_Ki = 0.5,                   //
                              coef_Kd = 0,                   //
                              nb_tic_per_deg = 43.3  ,
                                end_stop_type  = 1  
);
//poignet
mbed_hall_driven_motor motor_poignet(encoder4.count ,                         //count_2_pin
                                stop_pin = A3,
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
                                coef_Ki = 0.1, //
                                coef_Kd = 0,  //
                                nb_tic_per_deg = 38.8 ,
                                end_stop_type  = 1  );

//poignet haut
// DigitalOut pinPB4(PA_6,0);
// DigitalOut pinPA2(PA_2,0);
mbed_hall_driven_motor motor_poignet_haut(encoder5.count ,                    //count_2_pin
                                stop_pin = A4  ,
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



/****************************   MAIN *********************************************************/
//petit doigt
mbed_current_driven_motor doigt_5 (current1,
                                    pwm1,
                                    11, //pin_IN1
                                    10, //pin_IN2
                                    3, //current_sensor_chanel
                                    "doigt_5" ,//motor_name
                                    FLAG_START_DOIGT_5, //flag_start
                                    FLAG_STOP_DOIGT_5 ,//flag_stop
                                    100 //current_limit
                                    ,2000 //nominal_speed
                                    );

//annulaire 
mbed_current_driven_motor doigt_4 (current1,
                                    pwm1,
                                    9, //pin_IN1
                                    8, //pin_IN2
                                    2, //current_sensor_chanel
                                    "doigt_4" ,//motor_name
                                    FLAG_START_DOIGT_4, //flag_start
                                    FLAG_STOP_DOIGT_4 ,//flag_stop
                                    150 //current_limit
                                    ,2000 //nominal_speed
                                    );

//annulaire 
mbed_current_driven_motor doigt_3 (current1,
                                    pwm1,
                                    7, //pin_IN1
                                    6, //pin_IN2
                                    1, //current_sensor_chanel
                                    "doigt_3" ,//motor_name
                                    FLAG_START_DOIGT_3, //flag_start
                                    FLAG_STOP_DOIGT_3 ,//flag_stop
                                    150 //current_limit
                                    ,2000 //nominal_speed
                                    );
//index 
mbed_current_driven_motor doigt_2 (current,
                                    pwm1,
                                    4, //pin_IN1
                                    5, //pin_IN2
                                    1, //current_sensor_chanel
                                    "doigt_2" ,//motor_name
                                    FLAG_START_DOIGT_2, //flag_start
                                    FLAG_STOP_DOIGT_2 ,//flag_stop
                                    100 //current_limit
                                    ,2000 //nominal_speed
                                    );                                    
//pouce 
mbed_current_driven_motor doigt_1 (current,
                                    pwm1,
                                    3, //pin_IN1
                                    2, //pin_IN2
                                    2, //current_sensor_chanel
                                    "doigt_1" ,//motor_name
                                    FLAG_START_DOIGT_1, //flag_start
                                    FLAG_STOP_DOIGT_1 ,//flag_stop
                                    100 //current_limit
                                    ,2000 //nominal_speed
                                    );                                     
//pouce bas
mbed_current_driven_motor doigt_0 (current,
                                    pwm1,
                                    0, //pin_IN1
                                    1, //pin_IN2
                                    3, //current_sensor_chanel
                                    "doigt_0" ,//motor_name
                                    FLAG_START_DOIGT_0, //flag_start
                                    FLAG_STOP_DOIGT_0 ,//flag_stop
                                    10, //current_limit
                                    4095 //nominal_speed
                                    );                                       