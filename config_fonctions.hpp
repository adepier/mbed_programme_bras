/*  
  
 */
#include "config_motor.hpp"

 
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
      { move_arm ((int)(mail->data_1*360.0/255.0),
                  (int)(mail->data_2*360.0/255.0),
                  (int)(mail->data_3*360.0/255.0),
                  (int)(mail->data_4*360.0/255.0),
                  (int)(mail->data_5*360.0/255.0));
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
      if (mail->cmde == 11){motor_poignet_haut.init();}
      if (mail->cmde == 12){
                            // si le flag stop est 1 on bouge de 5 degrés pour avoir flag stop à 0
                            //if ()
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
                            //ouvre la main
                            open_hand();
                            }
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


