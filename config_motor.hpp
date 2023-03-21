/*  
  
 */
#include "mbed_PWMServoDriver.h"
#include "mbed_hall_driven_motor.h"
#include "mbed.h"
#include "mbed_current_driven_motor.h"
#include <list>
 #include "pin_encoder.h" 


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


EventFlags event_flag;

/****************************   BRAS  *******************************************************************************/

encoder_values encoder1;
encoder_values encoder2;
encoder_values encoder3;
encoder_values encoder4;
encoder_values encoder5;

// Nucleo L432KC
// #define I2C_SDA D0
// #define I2C_SCL D1
// pin_encoder counter1_1(D2,1,encoder1 );
// pin_encoder counter1_2(D3,2,encoder1 );
// pin_encoder counter2_1(D5,1,encoder2 );
// pin_encoder counter2_2(D4,2,encoder2 );
// pin_encoder counter3_1(D6,1,encoder3 );
// pin_encoder counter3_2(D9,2,encoder3 );
// pin_encoder counter4_1(D11,1,encoder4 );
// pin_encoder counter4_2(D10,2,encoder4 );
// pin_encoder counter5_1(D12,1,encoder5 );
// pin_encoder counter5_2(D13,2,encoder5 );
 //L452RE
 #define I2C_SDA PB_9
 #define I2C_SCL PB_8
 #define STOP_PIN PB_3
pin_encoder counter1_1(PA_6,1,encoder1 );
pin_encoder counter1_2(PA_5,2,encoder1 );
pin_encoder counter2_1(PA_7,2,encoder2 );
pin_encoder counter2_2(PB_2,1,encoder2 );
pin_encoder counter3_1(PB_1,1,encoder3 );
pin_encoder counter3_2(PB_15,2,encoder3 );
pin_encoder counter4_1(PB_13,1,encoder4 );
pin_encoder counter4_2(PB_14,2,encoder4 );
pin_encoder counter5_1(PB_10,2,encoder5 );
pin_encoder counter5_2(PB_4,1,encoder5 );
// CAN
CAN can(PA_11, PA_12);
 // Définir l'ID de l'accusé de réception à envoyer
#define ACK_ID 0x101
// Définir l'ID du materiel
#define DEVICE_ID 4

I2C i2c(I2C_SDA, I2C_SCL);


mbed_PWMServoDriver pwm(0x40, i2c); // Carte d'extension 16 sorties pour le pilotage des moteurs du bras
mbed_PWMServoDriver pwm1(0x41, i2c);  // deuxieme carte d'extension 16 sorties pour le pilotage de la main                                      
INA3221 current(i2c,0x42);
INA3221 current1(i2c,0x43);

//epaule à plat
mbed_hall_driven_motor motor_epaule_a_plat(encoder1.count ,                           //count_2_pin 
                              stop_pin = STOP_PIN,               //pin de fin de course
                              pwm,                            //carte pwm
                              forward_or_dir_pin = 0,        //pin de commande de la direction du moteur
                              backward_or_speed_pin = 1,     //pin de commande de la vitesse du moteur 
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
                              nb_tic_per_deg = 21.65  ,
                              end_stop_type  = 1 ,
                                false /*reverse rotation*/ 
);
//epaule haut
mbed_hall_driven_motor motor_epaule_haut(encoder2.count ,                        //count_2_pin
                              stop_pin = STOP_PIN,               //pin de fin de course
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
                              coef_Ki = 0.1,                   //
                              coef_Kd = 0,                   //
                              nb_tic_per_deg = 20 ,
                                end_stop_type  = 1 ,
                                false /*reverse rotation*/ 
);
//coude
mbed_hall_driven_motor motor_coude( encoder3.count ,                        //count_2_pin
                              stop_pin = STOP_PIN,               //pin de fin de course
                              pwm,                           //carte pwm
                              forward_or_dir_pin = 4,        //pin de commande de la direction du moteur
                              backward_or_speed_pin = 5,     //pin de commande de la vitesse du moteur 
                              motor_name = "Coude",          //nom du moteur
                              motor_shield_type = 1,         // motor_shield_type:1=type dir/pwm -- 2=type Forward/backward
                              flag_start = FLAG_START_COUDE, //
                              flag_stop = FLAG_STOP_COUDE,   //
                              init_speed = 2500,             //
                              min_speed = 500,                 //
                              max_speed = 4095,              //
                              coef_Kp = 0.5,                   //
                              coef_Ki = 0.5,                   //
                              coef_Kd = 0,                   //
                              nb_tic_per_deg = 21.65  ,
                                end_stop_type  = 1 ,
                                false /*reverse rotation*/ 
);
//poignet
mbed_hall_driven_motor motor_poignet(encoder4.count ,                         //count_2_pin
                                stop_pin = STOP_PIN,
                                pwm,
                                forward_or_dir_pin = 6,
                                backward_or_speed_pin = 7, 
                                motor_name = "Poignet",
                                motor_shield_type = 1,
                                flag_start = FLAG_START_POIGNET,
                                flag_stop = FLAG_STOP_POIGNET,
                                init_speed = 3500,
                                min_speed =3500,
                                max_speed = 4095,
                                coef_Kp = 3,  //
                                coef_Ki = 1, //
                                coef_Kd = 0,  //
                                nb_tic_per_deg = 18 ,
                                end_stop_type  = 1 ,
                                true /*reverse rotation*/ );

//poignet haut
// DigitalOut pinPB4(PA_6,0);
// DigitalOut pinPA2(PA_2,0);
mbed_hall_driven_motor motor_poignet_haut(encoder5.count ,                    //count_2_pin
                                stop_pin = STOP_PIN ,
                                pwm,
                                forward_or_dir_pin = 8,
                                backward_or_speed_pin = 9, 
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
                                end_stop_type  = 1,
                                true /*reverse rotation*/);



/****************************   MAIN *********************************************************/
//petit doigt
mbed_current_driven_motor doigt_5 (current1,
                                    pwm1,
                                    10, //pin_IN1
                                    11, //pin_IN2
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
                                    2, //pin_IN1
                                    3, //pin_IN2
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
 


//###########################
//           Fonction Ouvre la main
//##########################

void open_hand()
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
}
//###########################
//           Fonction Ouvre la main sans le pouce
//##########################

void open_hand_without_thumb()
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
     
}
//###########################
//           Fonction ferme le pouce
//##########################

void close_thumb()
{
  
    doigt_0.set_target_to_close_to_endstop(); //on ferme la pince
      event_flag.set(FLAG_START_DOIGT_0); // démarre les moteurs 
    event_flag.wait_all(FLAG_STOP_DOIGT_0); // attend que les moteurs
    
    printf("la pince est fermée\n  " );
     
}
//###########################
//           Fonction ferme la main
//##########################

void close_hand()
{
  
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
}
//###########################
//           Fonction signe ILY de la main
//##########################

void hand_ILY_from_open()
{  
  
     // on définit la nouvelle cible 
    doigt_2.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_3.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_4.set_target_to_close_and_stop() ;// on ferme le doigt et stop 
    printf("on ferme et stop\n" );
    event_flag.set( FLAG_START_DOIGT_2 |  FLAG_START_DOIGT_3 | FLAG_START_DOIGT_4  );     // démarre les moteurs
    event_flag.wait_all( FLAG_STOP_DOIGT_2 |FLAG_STOP_DOIGT_3 | FLAG_STOP_DOIGT_4  ); // attend que les moteurs
    
    printf("le doigt est fermé\n stop 5sec...\n" );
}

//###########################
//           Fonction bouger le bras
//##########################
//position moteurs
 int last_target_epaule_a_plat = 0 ;
  int last_target_epaule_haut = 0 ;
  int last_target_coude = 0 ;
  int last_target_poignet = 0 ;
  int last_target_poignet_haut = 0 ;

void move_arm( int epaule_a_plat,int epaule_haut,int coude,int poignet, int poignet_haut )
{  
  uint32_t flags_start=0;
  uint32_t flags_stop=0;

 
  
   
  //définit les moteur a faire tourner
if (last_target_epaule_a_plat !=epaule_a_plat){
  flags_start = flags_start|FLAG_START_EPAULE_A_PLAT; //flags start
  flags_stop = flags_stop | FLAG_STOP_EPAULE_A_PLAT; //flags stop
  last_target_epaule_a_plat =epaule_a_plat; //retient la position
  motor_epaule_a_plat._target = epaule_a_plat;  // définit les target
  }
if (last_target_epaule_haut !=epaule_haut){
  flags_start = flags_start|FLAG_START_EPAULE_HAUT; //flags start
  flags_stop = flags_stop | FLAG_STOP_EPAULE_HAUT; //flags stop
  last_target_epaule_haut =epaule_haut; //retient la position
  motor_epaule_haut._target = epaule_haut;  // définit les target
  }
if (last_target_coude !=coude){
  flags_start = flags_start|FLAG_START_COUDE; //flags start
  flags_stop = flags_stop | FLAG_STOP_COUDE; //flags stop
  last_target_coude =coude; //retient la position
  motor_coude._target = coude;  // définit les target
  }
if (last_target_poignet !=poignet){
  flags_start = flags_start|FLAG_START_POIGNET; //flags start
  flags_stop = flags_stop | FLAG_STOP_POIGNET; //flags stop
  last_target_poignet= poignet; //retient la position
  motor_poignet._target = poignet; // définit les target
  }
    
if (last_target_poignet_haut !=poignet_haut){
    flags_start = flags_start | FLAG_START_POIGNET_HAUT; //flags start
    flags_stop = flags_stop | FLAG_STOP_POIGNET_HAUT; //flags stop
  last_target_poignet_haut =poignet_haut;//retient la position
  motor_poignet_haut._target = poignet_haut; // définit les target
  }   
 

  printf("démarre les moteurs \n"); 
 
  event_flag.set(flags_start);  // démarre les moteurs 
  event_flag.wait_all(flags_stop ); // attend que les moteurs
 
}



//###########################
//           THREAD
//##########################
//Threads moteur
Thread thread_motor_coude(osPriorityNormal,1000, nullptr,  nullptr);
Thread thread_motor_poignet(osPriorityNormal,1000, nullptr,  nullptr);
Thread thread_motor_epaule_a_plat(osPriorityNormal,1000, nullptr,  nullptr);
Thread thread_motor_epaule_haut(osPriorityNormal,1000, nullptr,  nullptr);
Thread thread_motor_poignet_haut(osPriorityNormal,1000, nullptr,  nullptr);

//Threads moteur main
Thread thread_motor_doigt0;
Thread thread_motor_doigt1;
Thread thread_motor_doigt2;
Thread thread_motor_doigt3;
Thread thread_motor_doigt4;
Thread thread_motor_doigt5;

/*pour l'execution des commandes CAN*/
Thread thread_execute_cmde;
/* Mail */
typedef struct
{
  int cmde; /* commande*/
  /*parametres*/
  int data_1; /* commande_1 : epaule bas ou main*/
  int data_2; /* commande_2 : epaule haut*/
  int data_3; /* commande_3 : coude*/
  int data_4; /* commande_4 : poignet bas*/
  int data_5; /* commande_5 : poignet haut*/ 
  int data_6; /* commande_6 : spare*/ 
} mail_t;

Mail<mail_t, 16> mail_box;

void run_motor_in_thread(mbed_hall_driven_motor *motor)
{
  while (true)
  {
    event_flag.wait_all(motor->_flag_start); // attend le flag de démarrage 
    motor->run();
    event_flag.set(motor->_flag_stop);
     
  }
}

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

 
 /*******
 * Fonctions BRAS
Message:
ID = destinataire
Data[0] = emetteur
Data[1..7] = valeurs

ID = 1 -> commande pour le carrousel 1
ID = 2 -> commande pour le carrousel 2
ID = 3 -> commande pour le carrousel 3
ID = 4 -> commande pour le bras

cmde = 1 -> move_arm (parametres: angles = DATA[1..5]) 
cmde = 2 -> open_hand
cmde = 3 -> open_hand_without_thumb
cmde = 4 -> close_thumb
cmde = 5 -> close_hand
cmde = 6 -> hand_ILY_from_open
cmde = 7 -> init epaule bas
cmde = 8 -> init epaule haut
cmde = 9 -> init coude  
cmde = 10 -> init poignet bas  
cmde = 11 -> init poignet haut
cmde = 12 -> 
cmde = 13 -> 
cmde = 100 -> donne la position bras/ renvoie les angles = data_8..3  (parametres: destinataire data_1  )
cmde = 101 -> donne la position main/ renvoie ouvert/fermé = data_2 (parametres: destinataire data_1  )
********/

void run_cmde_CAN_in_thread()
{
  while (true)
  {
    mail_t *mail = mail_box.try_get();

    if (mail != nullptr)
    { printf(" get cmde %i \n", mail->cmde);
      if (mail->cmde == 1 )
      { move_arm (mail->data_1,mail->data_2,mail->data_3,mail->data_4,mail->data_5);
        printf(" position bras %i, %i, %i, %i, %i \n", mail->data_1,mail->data_2,mail->data_3,mail->data_4,mail->data_5);}
      if (mail->cmde == 2){open_hand();}
      if (mail->cmde == 3){open_hand_without_thumb();}
      if (mail->cmde == 4){close_thumb();}
      if (mail->cmde == 5){close_hand();}
      if (mail->cmde == 6){hand_ILY_from_open();}
      if (mail->cmde == 7){motor_epaule_a_plat.init();}
      if (mail->cmde == 8){motor_epaule_haut.init();}
      if (mail->cmde == 9){motor_coude.init();}
      if (mail->cmde == 10){motor_poignet.init();}
      if (mail->cmde == 11){motor_epaule_a_plat.init();}

      mail_box.free(mail);
    }

    // ThisThread::sleep_for(chrono::milliseconds(1s));
  }
}


//###########################
//           INIT
//##########################

void init_all(){
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
  
  
  // init coude
  motor_coude.init();
  // init epaule_haut
  motor_epaule_haut.init();
  // init epaule_a_plat
  motor_poignet_haut.init(); 
  // init poignet
  motor_poignet.init();
// init epaule_a_plat
  motor_epaule_a_plat.init();
 
  
 //init INA3221
  current.SetAveragingMode(INA3221_AVERAGE_256);
  current.SetShuntConversionTime(INA3221_588_US);
  current.SetBusConversionTime(INA3221_140_US);

  current1.SetAveragingMode(INA3221_AVERAGE_256);
  current1.SetShuntConversionTime(INA3221_588_US);
  current1.SetBusConversionTime(INA3221_140_US);

 //démarrage des doigts
  thread_motor_doigt0.start(callback(run_motor_doigt_in_thread, &doigt_0)); 
  thread_motor_doigt1.start(callback(run_motor_doigt_in_thread, &doigt_1)); 
  thread_motor_doigt2.start(callback(run_motor_doigt_in_thread, &doigt_2)); 
  thread_motor_doigt3.start(callback(run_motor_doigt_in_thread, &doigt_3)); 
  thread_motor_doigt4.start(callback(run_motor_doigt_in_thread, &doigt_4)); 
  thread_motor_doigt5.start(callback(run_motor_doigt_in_thread, &doigt_5));
 
//démarrage des moteurs
  thread_motor_coude.start(callback(run_motor_in_thread, &motor_coude));
  thread_motor_poignet.start(callback(run_motor_in_thread, &motor_poignet));
  thread_motor_epaule_a_plat.start(callback(run_motor_in_thread, &motor_epaule_a_plat));
  thread_motor_epaule_haut.start(callback(run_motor_in_thread, &motor_epaule_haut));
  thread_motor_poignet_haut.start(callback(run_motor_in_thread, &motor_poignet_haut));

  //synchronise les moteurs
    motor_coude.set_speed_sync(&motor_epaule_a_plat);
    motor_coude.set_speed_sync(&motor_epaule_haut);
    motor_coude.set_speed_sync(&motor_poignet);
    motor_coude.set_speed_sync(&motor_poignet_haut);

    motor_epaule_haut.set_speed_sync(&motor_epaule_a_plat);
    motor_epaule_haut.set_speed_sync(&motor_coude);
    motor_epaule_haut.set_speed_sync(&motor_poignet);
    motor_epaule_haut.set_speed_sync(&motor_poignet_haut);

    motor_epaule_a_plat.set_speed_sync(&motor_coude);
    motor_epaule_a_plat.set_speed_sync(&motor_epaule_haut);
    motor_epaule_a_plat.set_speed_sync(&motor_poignet);
    motor_epaule_a_plat.set_speed_sync(&motor_poignet_haut);

    motor_poignet.set_speed_sync(&motor_coude);
    motor_poignet.set_speed_sync(&motor_epaule_haut);
    motor_poignet.set_speed_sync(&motor_epaule_a_plat); 
    motor_poignet.set_speed_sync(&motor_poignet_haut);   

    motor_poignet_haut.set_speed_sync(&motor_coude);
    motor_poignet_haut.set_speed_sync(&motor_epaule_haut);
    motor_poignet_haut.set_speed_sync(&motor_epaule_a_plat); 
    motor_poignet_haut.set_speed_sync(&motor_poignet); 
  // motor_coude.set_speed_sync_2(angle_epaule_a_plat,true,true);

  //commandes CAN 
  thread_execute_cmde.start(callback(run_cmde_CAN_in_thread));
}


