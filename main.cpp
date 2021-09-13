/*  
  
 */
#include "adafruit_pwmservodriver/Adafruit_PWMServoDriver.h"
#include "hall_driven_motor/hall_driven_motor.h"
//#include "NamedType/include/NamedType/named_type.hpp"
//#include "hall_driven_motor/MotorConfig/MotorConfig.h"
#include "mbed.h"
// using FirstName = fluent::NamedType<std::string, struct FirstNameTag>;
// using LastName = fluent::NamedType<std::string, struct LastNameTag>;

// static const FirstName::argument firstName;
// static const LastName::argument lastName;
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

#define FLAGS_START (FLAG_START_POIGNET | FLAG_START_COUDE) // this is a bit-wise OR (not a logical OR)
#define FLAGS_STOP (FLAG_STOP_POIGNET | FLAG_STOP_COUDE)    // this is a bit-wise OR (not a logical OR)

#define FLAG_START 1
#define FLAG_STOP 2
#define MOTOR_SHIELD_TYPE 1 // motor_shield_type:1=type dir/pwm -- 2=type Forward/backward

// //#####COUDE
// MotorConfig motor_config_coude ( count_pin = PB_6,     //pin compteur de tour
//                   stop_pin=PB_7,      //pin de fin de course
//                   forward_or_dir_pin=0,       //pin de commande de la direction du moteur
//                   backward_or_speed_pin=1,      //pin de commande de la vitesse du moteur
//                   motor_name = 'C',       //nom du moteur
//                   motor_shield_type =1 , //motor_shield_type:1=type dir/speed -- 2=type Forward/backward
//                   flag_start= (1UL << 0) // 00000000000000000000000000000001,    //numéro du flag start
//                   flag_stop=(1UL << 1)  // 00000000000000000000000000000010,     //numéro du flag stop
//                   init_speed=1500,        //vitesse pour l'initialisation
//                   min_speed =0,         //vitesse mini
//                   max_speed = 4500,         //vitesse maxi 
//                   coef_Kp = 1,        //PID coef proportionnel
//                   coef_Ki = 1,        //PID coef integral
//                   coef_Kd= 0,        //PID coef dérivée
//                   nb_tic_per_deg = 43.3  // nombre de tic par tour pour calculer l'angle ) 
 
// //#####POIGNET
// MotorConfig motor_config_poignet ( count_pin = PA_8,     //pin compteur de tour
//                   stop_pin=PB_1,      //pin de fin de course
//                   forward_or_dir_pin=10,       //pin de commande de la direction du moteur
//                   backward_or_speed_pin=11,      //pin de commande de la vitesse du moteur
//                   motor_name = 'C',       //nom du moteur
//                   motor_shield_type =1 , //motor_shield_type:1=type dir/speed -- 2=type Forward/backward
//                   flag_start= (1UL << 2) // 00000000000000000000000000000100,    //numéro du flag start
//                   flag_stop=(1UL << 3)  // 00000000000000000000000000001000,     //numéro du flag stop
//                   init_speed=2500,        //vitesse pour l'initialisation
//                   min_speed =0,         //vitesse mini
//                   max_speed = 4500,         //vitesse maxi 
//                   coef_Kp = 1,        //PID coef proportionnel
//                   coef_Ki = 15,        //PID coef integral
//                   coef_Kd= 0,        //PID coef dérivée
//                   nb_tic_per_deg = 38.8  // nombre de tic par tour pour calculer l'angle ) 


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


//coude
hall_driven_motor motor_coude( count_pin = PB_6,       //pin compteur de tour
                              PIN_STOP_COUDE,        //pin de fin de course
                              pwm,                   //carte pwm
                              PIN_DIR_COUDE,         //pin de commande de la direction du moteur
                              PIN_PWM_COUDE,         //pin de commande de la vitesse du moteur
                              target_angle_coude,    //angle cible (pointeur)
                              angle_coude,           //angle en cours  (pointeur), il faut en faire une variable gloable pour l'utiliser dans les autres moteurs
                              angle_poignet,         //angle a suivre (pointeur)
                              'C',                   //nom du moteur
                              MOTOR_SHIELD_TYPE,     //
                              FLAG_START_COUDE,      //
                              FLAG_STOP_COUDE,       //
                              INIT_SPEED_COUDE,      //
                              MIN_MOTOR_SPEED_COUDE, //
                              MAX_MOTOR_SPEED_COUDE, //
                              COEF_ACCEL_COUDE,      //
                              COEF_KP_COUDE,         //
                              COEF_KI_COUDE,         //
                              COEF_KD_COUDE,         //
                              NB_TIC_PER_DEG_COUDE   //
);
//poignet
hall_driven_motor motor_poignet(count_pin = PIN_COUNT_POIGNET, PIN_STOP_POIGNET, pwm, PIN_DIR_POIGNET, PIN_PWM_POIGNET, target_angle_poignet, angle_poignet, angle_coude, 'P', MOTOR_SHIELD_TYPE, FLAG_START_POIGNET, FLAG_STOP_POIGNET, INIT_SPEED_POIGNET, MIN_MOTOR_SPEED_POIGNET, MAX_MOTOR_SPEED_POIGNET, COEF_ACCEL_POIGNET, COEF_KP_POIGNET, COEF_KI_POIGNET, COEF_KD_POIGNET, NB_TIC_PER_DEG_POIGNET);

//Threads moteur
Thread thread_motor_coude;
Thread thread_motor_poignet;

//###########################
//           INIT
//##########################
void init()
{
  // Call site
motor_coude.displayName( firstName = "John",  lastName = "Doe"); 

  printf("init PWM\n");
  pwm.begin();
  pwm.setPWMFreq(1600); // This is the maximum PWM frequency
  // RAZ pwm
  for (uint8_t pwmnum = 0; pwmnum < 16; pwmnum++)
  {
    pwm.setPWM(pwmnum, 0, 0);
  }

  printf("init poignet\n");
  motor_poignet.init();

  printf("init coude\n");
  motor_coude.init();
  
}

//###########################
//           THREAD
//##########################

void run_motor_in_thread(hall_driven_motor *motor)
{
  while (true)
  {
    event_flag.wait_all(motor->get_flag_start()); // attend que le moteur ai le flag de démarrage
    // printf("start motor %c \n",(char) motor->get_motor_name());
    motor->run();
    event_flag.set(motor->get_flag_stop());
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
  thread_motor_coude.start(callback(run_motor_in_thread, &motor_coude));
  thread_motor_poignet.start(callback(run_motor_in_thread, &motor_poignet));

  //on met les moteur en place pour la premiere fois
  target_angle_poignet = int(2);      //point haut poignet
  target_angle_coude = int((2 + 87)); //+87 deg sur le coude pour être à l'horizontal
  event_flag.set(FLAGS_START);
  event_flag.wait_all(FLAGS_STOP); // attend que les moteurs
  printf("commande \t reponse \n");

  //demarrage la synchro des moteurs
  motor_coude.set_speed_sync(true);
  motor_poignet.set_speed_sync(false);

  while (true)
  {
    int deplacement = 90;
    int angle_poignet_init = int(2); //point haut poignet
    int angle_coude_init = int(89);  //point haut poignet

    target_angle_poignet = angle_poignet_init + deplacement; //--> point bas le moteur fait 2->92
    target_angle_coude = angle_coude_init + deplacement;     //--> point bas le moteur fait 89->177     (+87 deg sur le coude pour être à l'horizontal)
                                                             // démarre les moteurs
    event_flag.set(FLAGS_START);
    event_flag.wait_all(FLAGS_STOP); // attend que les moteurs

    printf("fin descente \n   ");
    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));

    // on définit la nouvelle cible
    angle_poignet_init = angle_poignet;                      //point haut poignet
    angle_coude_init = angle_coude;                          //point haut poignet
    target_angle_poignet = angle_poignet_init - deplacement; //--> point haut le moteur fait 92->2
    target_angle_coude = angle_coude_init - deplacement;     //--> point bas le moteur fait 177->89    (+87 deg sur le coude pour être à l'horizontal)

    // démarre les moteurs
    event_flag.set(FLAGS_START);
    event_flag.wait_all(FLAGS_STOP); // attend que les moteurs
    printf("fin montée \n");

    // on attend un peu
    ThisThread::sleep_for(chrono::milliseconds(1000));
  }
}
