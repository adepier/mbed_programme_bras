/*  
  
 */
#include "adafruit_pwmservodriver/Adafruit_PWMServoDriver.h"
#include "hall_driven_motor/hall_driven_motor.h"
#include "mbed.h"

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

#define FLAG_START_COUDE (1UL << 0) // 00000000000000000000000000000001
#define FLAG_STOP_COUDE (1UL << 1)  // 00000000000000000000000000000010
//#####POIGNET
#define FLAG_START_POIGNET (1UL << 2) // 00000000000000000000000000000100
#define FLAG_STOP_POIGNET (1UL << 3)  // 00000000000000000000000000001000

double target_angle_coude = 0;
double target_angle_poignet = 0;

double angle_coude;
double angle_poignet;

int flag_speed_sync_coude;

I2C i2c(I2C_SDA, I2C_SCL);

EventFlags event_flag;

Adafruit_PWMServoDriver pwm(0x40, i2c); // Carte d'extension 16 sorties pour le pilotage de servos
                                        // en PWM  (adresse I2C par defaut 0x40)
                                        //epaule

//coude
hall_driven_motor motor_coude(count_pin = PB_6,              //pin compteur de tour
                              stop_pin = PB_7,               //pin de fin de course
                              pwm,                           //carte pwm
                              forward_or_dir_pin = 0,        //pin de commande de la direction du moteur
                              backward_or_speed_pin = 1,     //pin de commande de la vitesse du moteur
                              target_angle_coude,            //angle cible (pointeur = variable globale)
                              angle_coude,                   //angle en cours  (pointeur = variable globale), il faut en faire une variable gloable pour l'utiliser dans les autres moteurs 
                              motor_name = "Coude",          //nom du moteur
                              motor_shield_type = 1,         // motor_shield_type:1=type dir/pwm -- 2=type Forward/backward
                              flag_start = FLAG_START_COUDE, //
                              flag_stop = FLAG_STOP_COUDE,   //
                              init_speed = 1500,             //
                              min_speed = 500,                 //
                              max_speed = 4095,              //
                              coef_Kp = 1,                   //
                              coef_Ki = 10,                   //
                              coef_Kd = 0,                   //
                              nb_tic_per_deg = 43.3          //
);
//poignet
hall_driven_motor motor_poignet(count_pin = PA_8,
                                stop_pin = PB_1,
                                pwm,
                                forward_or_dir_pin = 10,
                                backward_or_speed_pin = 11,
                                target_angle_poignet,
                                angle_poignet, 
                                motor_name = "Poignet",
                                motor_shield_type = 1,
                                flag_start = FLAG_START_POIGNET,
                                flag_stop = FLAG_STOP_POIGNET,
                                init_speed = 3500,
                                min_speed =500,
                                max_speed = 4095,
                                coef_Kp = 10,  //
                                coef_Ki = 500, //
                                coef_Kd = 0,  //
                                nb_tic_per_deg = 38.8);



