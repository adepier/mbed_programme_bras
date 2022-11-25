#include "config_motor.hpp"

//Threads moteur
Thread thread_motor_coude;
Thread thread_motor_poignet;
Thread thread_motor_epaule_a_plat;
Thread thread_motor_epaule_haut;
Thread thread_motor_poignet_haut;
//Threads moteur main
Thread thread_motor_doigt0;
Thread thread_motor_doigt1;
Thread thread_motor_doigt2;
Thread thread_motor_doigt3;
Thread thread_motor_doigt4;
Thread thread_motor_doigt5;

//position moteurs
 int last_target_epaule_a_plat = 0 ;
  int last_target_epaule_haut = 0 ;
  int last_target_coude = 0 ;
  int last_target_poignet = 0 ;
  int last_target_poignet_haut = 0 ;

//###########################
//           THREAD
//##########################

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
}

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
   flags_start = flags_start|FLAG_START_POIGNET_HAUT; //flags start
   flags_stop = flags_stop | FLAG_STOP_POIGNET_HAUT; //flags stop
  last_target_poignet_haut =poignet_haut;//retient la position
  motor_poignet_haut._target = poignet_haut; // définit les target
  }   
 

  printf("démarre les moteurs \n"); 
  event_flag.set(flags_start);  // démarre les moteurs 
  event_flag.wait_all(flags_stop ); // attend que les moteurs
}
//###########################
//           MAIN
//##########################
int main()
{

   // initialisation
  init();
 

  while (true)
  {
    

//on met la main a l'horizontale
  move_arm (0,0,0,35,160);
//on ouvre la main
 open_hand();
 
printf("on baisse le bras pour prendre le verre \n"); 

 move_arm (20,25,230,240,160);
//ferme le pouce
 close_thumb(); 
  
  move_arm (0,25,190,200,160);
  move_arm (0,5,150,180,160);
  //attrappe le verre
close_hand();

//on releve de 150 (75 coude/75 epaule)
printf("on releve le bras \n"); 
// move_arm (0,15,20,70,160);
//on releve de 150 (75 coude/75 epaule)
//printf("on releve le bras \n"); 
 //move_arm (0,90,95,40,160);
//on releve de 150 (75 coude/75 epaule)
//printf("on releve le bras \n"); 
 move_arm (120,90,95,40,160);
// on attend un peu
  ThisThread::sleep_for(chrono::milliseconds(1000));
//on repose le verre
move_arm (0,15,150,170,160);
 move_arm (0,5,150,180,160);
 open_hand_without_thumb();
 move_arm (0,20,190,205,160);
   move_arm (20,25,230,240,160);
//on releve de 150 (75 coude/75 epaule)
printf("on releve le bras \n"); 
 //move_arm (120,90,95,90,160);
// hand_ILY_from_open();
// on attend un peu
// ThisThread::sleep_for(chrono::milliseconds(5000));

  //   motor_poignet._target = (motor_poignet._angle + deplacement_poignet); //--> point bas le moteur fait 2->92
  //   motor_coude._target = (motor_coude._angle + deplacement_coude);       //--> point bas le moteur fait 89->177     (+87 deg sur le coude pour être à l'horizontal)
  //   motor_epaule_a_plat._target = (motor_epaule_a_plat._angle + deplacement_epaule_a_plat);
  //   motor_epaule_haut._target = (motor_epaule_haut._angle + deplacement_epaule_haut);
    
  //   // motor_coude._debug_flag=true;
  //   // motor_epaule_haut._debug_flag=true; 
  //   printf("start descente \n");
  //    //démarrage des moteurs 
  //   event_flag.set(FLAG_START_COUDE |FLAG_START_POIGNET |  FLAG_START_EPAULE_A_PLAT | FLAG_START_EPAULE_HAUT);  // démarre les moteurs 
  //   event_flag.wait_all(FLAG_STOP_POIGNET | FLAG_STOP_COUDE | FLAG_STOP_EPAULE_A_PLAT | FLAG_STOP_EPAULE_HAUT); // attend que les moteurs 
  //   printf("fin descente \n");
  //   // on attend un peu
  //   ThisThread::sleep_for(chrono::milliseconds(1000));

  //   // on définit la nouvelle cible
  //   motor_poignet._target = (motor_poignet._angle - deplacement_poignet); //--> point bas le moteur fait 92->2
  //   motor_coude._target = (motor_coude._angle - deplacement_coude);       //--> point bas le moteur fait 177->89     (+87 deg sur le coude pour être à l'horizontal)
  //   motor_epaule_a_plat._target = (motor_epaule_a_plat._angle - deplacement_epaule_a_plat);
  //   motor_epaule_haut._target = (motor_epaule_haut._angle - deplacement_epaule_haut);

  //   printf("start montée \n");
     
  //   event_flag.set(FLAG_START_COUDE |FLAG_START_POIGNET |  FLAG_START_EPAULE_A_PLAT | FLAG_START_EPAULE_HAUT);  // démarre les moteurs
  //   event_flag.wait_all(FLAG_STOP_POIGNET | FLAG_STOP_COUDE | FLAG_STOP_EPAULE_A_PLAT | FLAG_STOP_EPAULE_HAUT); // attend que les moteurs
  //   printf("fin montée \n");

  //   // on attend un peu
  //   ThisThread::sleep_for(chrono::milliseconds(1000));
   }
}
