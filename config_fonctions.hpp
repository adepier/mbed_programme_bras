/*  
  
 */
#include "config_motor.hpp"
#include "debug_print.h" // Inclure le fichier d'en-tête contenant print_debug

 //position moteurs
 int last_target_epaule_a_plat = 0 ;
  int last_target_epaule_haut = 0 ;
  int last_target_coude = 0 ;
  int last_target_poignet = 0 ;
  int last_target_poignet_haut = 0 ;
int init_position_done =0; //0 : l'init n'a pas été faite / 1 l'init a été faite
/* current_position BRAS
0 : position_init 
1: prend_le_verre 
2: carrousel1
3: carrousel2 
4: carrousel3 
5: lache_le_verre
*/
int current_position = 0;
/*pour régler les positions du bras en debug*/
int reglage_bras_current_position = 0;
int fumee_lumiere_on = 0; //0=off 1=on
 
void move_arm( int epaule_a_plat,int epaule_haut,int coude,int poignet, int poignet_haut, int pCoef_accel )
{  
 
 

// mouvement_type:
// 0 : default, amortisseur au demarrage et PID à l'arrivée 
 
motor_epaule_a_plat._coef_accel = pCoef_accel;
motor_epaule_haut._coef_accel   = pCoef_accel;
motor_coude._coef_accel         = pCoef_accel;
motor_poignet._coef_accel       = pCoef_accel;
motor_poignet_haut._coef_accel  = pCoef_accel;
  

 uint32_t flags_start=0;
  uint32_t flags_stop=0; 
  //définit les moteurs à faire tourner
//if (last_target_epaule_a_plat != epaule_a_plat){
  flags_start = flags_start|FLAG_START_EPAULE_A_PLAT; //flags start
  flags_stop = flags_stop | FLAG_STOP_EPAULE_A_PLAT; //flags stop
  last_target_epaule_a_plat =epaule_a_plat; //retient la position
  motor_epaule_a_plat._target = epaule_a_plat;  // définit les target
   //print_debug("epaule_a_plat target: %i \n",epaule_a_plat); 
//   }
// if (last_target_epaule_haut !=epaule_haut){
  flags_start = flags_start|FLAG_START_EPAULE_HAUT; //flags start
  flags_stop = flags_stop | FLAG_STOP_EPAULE_HAUT; //flags stop
  last_target_epaule_haut =epaule_haut; //retient la position
  motor_epaule_haut._target = epaule_haut;  // définit les target
    //print_debug("epaule_haut target: %i \n",epaule_haut); 
//   }
// if (last_target_coude !=coude){
  flags_start = flags_start|FLAG_START_COUDE; //flags start
  flags_stop = flags_stop | FLAG_STOP_COUDE; //flags stop
  last_target_coude =coude; //retient la position
  motor_coude._target = coude;  // définit les target
     //print_debug("coude target: %i \n",coude); 
//   }
// if (last_target_poignet !=poignet){
  flags_start = flags_start|FLAG_START_POIGNET; //flags start
  flags_stop = flags_stop | FLAG_STOP_POIGNET; //flags stop
  last_target_poignet= poignet; //retient la position
  motor_poignet._target = poignet; // définit les target
     //print_debug("poignet target: %i \n",poignet); 
//   }  
// if (last_target_poignet_haut !=poignet_haut){
    flags_start = flags_start | FLAG_START_POIGNET_HAUT; //flags start
    flags_stop = flags_stop | FLAG_STOP_POIGNET_HAUT; //flags stop
  last_target_poignet_haut =poignet_haut;//retient la position
  motor_poignet_haut._target = poignet_haut; // définit les target
    //  print_debug("poignet_haut target: %i \n",poignet_haut); 
  // }   
 

 // print_debug("demarre les moteurs -> pPouvement_type: %i \n",pPouvement_type); 

  event_flag.clear();  //remet les flag à 0
  event_flag.set(flags_start);  // démarre les moteurs 
  event_flag.wait_all(flags_stop ); // attend que les moteurs terminent

print_debug("fin move \n" ); 

print_debug("motor_epaule_a_plat : target %f angle %f err: %f\n", motor_epaule_a_plat._target , motor_epaule_a_plat.get_angle(),motor_epaule_a_plat._target - motor_epaule_a_plat.get_angle());
print_debug("motor_epaule_haut   : target %f angle %f err: %f\n", motor_epaule_haut._target , motor_epaule_haut.get_angle(),motor_epaule_haut._target - motor_epaule_haut.get_angle());
print_debug("motor_coude         : target %f angle %f err: %f\n", motor_coude._target , motor_coude.get_angle(),motor_coude._target - motor_coude.get_angle());
print_debug("motor_poignet       : target %f angle %f err: %f\n", motor_poignet._target , motor_poignet.get_angle(),motor_poignet._target - motor_poignet.get_angle());
print_debug("motor_poignet_haut  : target %f angle %f err: %f\n", motor_poignet_haut._target , motor_poignet_haut.get_angle(),motor_poignet_haut._target - motor_poignet_haut.get_angle());
 
}
/*envoi les message CAN facilement!*/
 void send_buffer(int dest, int d1,int d2,int d3,int d4,int d5,int d6,int d7,int d8) {
    char buffer[8] ={0};
    buffer[0] = (char) d1;
    buffer[1] = (char) d2;
    buffer[2] = (char) d3;
    buffer[3] = (char) d4;
    buffer[4] = (char) d5;
    buffer[5] = (char) d6;
    buffer[6] = (char) d7;
    buffer[7] = (char) d8;
    CANMessage msg_to_send(dest,  buffer, 8); 
    print_debug("envoi :id: \t %i data: \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n", msg_to_send.id, msg_to_send.data[0], msg_to_send.data[1], msg_to_send.data[2], msg_to_send.data[3], msg_to_send.data[4], msg_to_send.data[5], msg_to_send.data[6], msg_to_send.data[7]);
     //essaye d'envoyer
    for( int i = 0; i <= 3; i++) 
    {if(can.write(msg_to_send)==0){  print_debug("error write %i\n", i ); 
              // très important! il faut attendre un peu avant de renvoyer le message!
              ThisThread::sleep_for(chrono::milliseconds(500ms));
                }  else {break;};}
        
}
//###########################
//           Fonction Ouvre la main
//##########################

void open_hand()
{
  //0 = ouvert; 1= fermé
   doigt_1.set_target_to_open() ;// on ouvre le doigt
   doigt_2.set_target_to_open() ;// on ouvre le doigt
   doigt_3.set_target_to_open() ;// on ouvre le doigt
   doigt_4.set_target_to_open() ;// on ouvre le doigt
   doigt_5.set_target_to_open() ;// on ouvre le doigt
    print_debug("on ouvre le doigt \n ");
    event_flag.set(FLAG_START_DOIGT_1 | FLAG_START_DOIGT_2 | FLAG_START_DOIGT_3 | FLAG_START_DOIGT_4 | FLAG_START_DOIGT_5); // démarre les moteurs 
    event_flag.wait_all(FLAG_STOP_DOIGT_1 | FLAG_STOP_DOIGT_2 | FLAG_STOP_DOIGT_3 | FLAG_STOP_DOIGT_4 | FLAG_STOP_DOIGT_5); // attend que les moteurs
    print_debug("le doigt est ouvert\n stop 5sec...\n" );
         // on définit la nouvelle cible
  doigt_0.set_target_to_open_to_endstop();  //on ouvre la pince
    event_flag.set(FLAG_START_DOIGT_0); // démarre les moteurs 
    event_flag.wait_all(FLAG_STOP_DOIGT_0); // attend que les moteurs
    
    print_debug("la pince est ouverte\n  " );
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
    print_debug("on ouvre le doigt \n ");
    event_flag.set(FLAG_START_DOIGT_1 | FLAG_START_DOIGT_2 | FLAG_START_DOIGT_3 | FLAG_START_DOIGT_4 | FLAG_START_DOIGT_5); // démarre les moteurs 
    event_flag.wait_all(FLAG_STOP_DOIGT_1 | FLAG_STOP_DOIGT_2 | FLAG_STOP_DOIGT_3 | FLAG_STOP_DOIGT_4 | FLAG_STOP_DOIGT_5); // attend que les moteurs
    
    print_debug("le doigt est ouvert\n stop 5sec...\n" );
     
}
//###########################
//           Fonction ferme le pouce
//##########################

void close_thumb()
{
  
    doigt_0.set_target_to_close_to_endstop(); //on ferme la pince
    event_flag.set(FLAG_START_DOIGT_0); // démarre les moteurs 
    event_flag.wait_all(FLAG_STOP_DOIGT_0); // attend que les moteurs
    
    print_debug("la pince est fermee\n  " );
     
}
void open_thumb()
{
  
    doigt_0.set_target_to_open_to_endstop(); //ouvre le pouce
    event_flag.set(FLAG_START_DOIGT_0); // démarre les moteurs 
    event_flag.wait_all(FLAG_STOP_DOIGT_0); // attend que les moteurs
    
    print_debug("la pince est fermee\n  " );
     
}
//###########################
//           Fonction ferme la main
//##########################

void close_hand()
{
  
    doigt_0.set_target_to_close_to_endstop(); //on ferme la pince
    event_flag.set(FLAG_START_DOIGT_0); // démarre les moteurs 
    event_flag.wait_all(FLAG_STOP_DOIGT_0); // attend que les moteurs
    print_debug("la pince est fermee\n  " );
     // on définit la nouvelle cible
    doigt_1.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_2.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_3.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_4.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_5.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    print_debug("on ferme et stop\n" );
    event_flag.set(FLAG_START_DOIGT_1 | FLAG_START_DOIGT_2 | FLAG_START_DOIGT_3 | FLAG_START_DOIGT_4 | FLAG_START_DOIGT_5);     // démarre les moteurs
    event_flag.wait_all(FLAG_STOP_DOIGT_1 | FLAG_STOP_DOIGT_2 | FLAG_STOP_DOIGT_3 | FLAG_STOP_DOIGT_4 | FLAG_STOP_DOIGT_5); // attend que les moteurs
    
    print_debug("le doigt est fermé\n stop 5sec...\n" );
}
void fumee_lumieres(){
  if(fumee_lumiere_on==1)
{send_buffer( 5, DEVICE_ID ,  1 ,0,   0,  0,  0,   0,   0 ); /*fumée*/ 
send_buffer (5, DEVICE_ID,200,0,0,0,0,0,0); /*envoi run sur le carrousel*/ 
// allume les leds
send_buffer( 1, DEVICE_ID ,  12 ,0,   0,  0,  0,   0,   0 ); /*demo led sur carrousel 3*/ 
send_buffer( 2, DEVICE_ID ,  12 ,0,   0,  0,  0,   0,   0 ); /*demo led sur carrousel 3*/ 
send_buffer( 3, DEVICE_ID ,  12 ,0,   0,  0,  0,   0,   0 ); /*demo led sur carrousel 3*/ 
send_buffer (1, DEVICE_ID,200,0,0,0,0,0,0); /*envoi run sur le carrousel*/ 
send_buffer (2, DEVICE_ID,200,0,0,0,0,0,0); /*envoi run sur le carrousel*/ 
send_buffer (3, DEVICE_ID,200,0,0,0,0,0,0); /*envoi run sur le carrousel*/ }

}
//###########################
//           Fonction bouger le bras
//##########################
// void move_position_verre_pour_lacher(){
// move_arm ( 248 ,   49 ,  218 ,  164 ,  183 , 1 );  // position verre pour lacher
// }
void move_position_init(){
   move_arm (   0   , 0        ,  0     ,0   ,  0       , 10       ); //INIT 
}
void move_position_thumb_down(){
   move_arm (   0   , 0        ,  0     ,0   ,  188       , 10       ); //position pouce en bas
}
void move_position_fuck(){
   move_arm (   0   , 0        ,  0     ,90   ,  0       , 10       ); //position fuck
}
void move_position_ily(){
   move_arm (   0   , 0        ,  0     ,56   ,  0       , 10       ); //position ily
}

void move_position_hors_carrousel(){
  move_arm ( 157 ,   102 ,  272 ,  0 ,  171 , 1 );  //tourne hors du carrousel 
}

void move_position_avant_verre(){
  move_arm (   192   , 93     ,  272  , -17   ,  171      , 1       );  //position avant verre
}
void move_position_verre(){
  //move_arm ( 244 ,   58 ,  232 ,  171 ,  188 , 1 ); // position verre
    move_arm ( 192 ,   39 ,  204 ,  0 ,  171 , 0 );   // position verre pour lacher
  // move_position_verre_pour_lacher();
}
void move_position_1(){
  fumee_lumieres();//fumée lumières
    move_arm ( 178 ,   73 ,  252 ,  -6 ,  171 , 0 );  // POS1  
}
void move_position_2(){
  fumee_lumieres();//fumée lumières
    move_arm ( 135 ,   195 ,  246 ,  112 ,  164 , 1 );  // POS2  
}
void move_position_3(){
     fumee_lumieres();//fumée lumières
    move_arm ( 0 ,   79 ,  269 ,  -20 ,  171 , 1 );// POS3  
}
//###########################
//           Fonction signe ILY de la main
//##########################

void hand_ILY_from_open()
{  
  //il faut que l'init soit faite et que le bras soit à l'init
   if (init_position_done ==1 && current_position == 0 ) {
   open_hand();
   //move_position_init(); //INIT au cas ou on  a bougé
     // on définit la nouvelle cible 
    doigt_2.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_3.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_4.set_target_to_close_and_stop() ;// on ferme le doigt et stop 
    print_debug("on ferme et stop\n" );
    event_flag.set( FLAG_START_DOIGT_2 |  FLAG_START_DOIGT_3 | FLAG_START_DOIGT_4  );     // démarre les moteurs
    move_position_ily();
    event_flag.wait_all( FLAG_STOP_DOIGT_2 |FLAG_STOP_DOIGT_3 | FLAG_STOP_DOIGT_4  ); // attend que les moteurs
    
    }
}
 

void thumb_up()
{ 
//il faut que l'init soit faite et que le bras soit à l'init
   if (init_position_done ==1 && current_position == 0 ) {
   open_hand();  
   
     // on définit la nouvelle cible 
    doigt_2.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_3.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_4.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_5.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    print_debug("on ferme et stop\n" );
    event_flag.set (FLAG_START_DOIGT_2 | FLAG_START_DOIGT_3 | FLAG_START_DOIGT_4 | FLAG_START_DOIGT_5);     // démarre les moteurs
    move_position_init(); //INIT au cas ou on  a bougé
    event_flag.wait_all(  FLAG_STOP_DOIGT_2 | FLAG_STOP_DOIGT_3 | FLAG_STOP_DOIGT_4 | FLAG_STOP_DOIGT_5); // attend que les moteurs
   }
}

void bouge_les_doigts()
{ 
//il faut que l'init soit faite et que le bras soit à l'init
   if (init_position_done ==1 && current_position == 0 ) {
   open_hand(); 
   move_position_init(); //INIT au cas ou on  a bougé
     // on définit la nouvelle cible 
    doigt_2.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    event_flag.set (FLAG_START_DOIGT_2  );  
    event_flag.wait_all(  FLAG_STOP_DOIGT_2  ); 
 print_debug("doigt 2 fermé / les autres ouverts\n  " );
    doigt_2.set_target_to_open() ;
    doigt_3.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    event_flag.set (FLAG_START_DOIGT_2 | FLAG_START_DOIGT_3) ;     // démarre les moteurs
    event_flag.wait_all(  FLAG_STOP_DOIGT_2 | FLAG_STOP_DOIGT_3 );
 print_debug("doigt 2 ouvert / 3 fermé\n  " );
    doigt_3.set_target_to_open() ;
    doigt_4.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    event_flag.set ( FLAG_START_DOIGT_3 | FLAG_START_DOIGT_4  );     // démarre les moteurs
    event_flag.wait_all(   FLAG_STOP_DOIGT_3 | FLAG_STOP_DOIGT_4  ); // attend que les moteurs
 print_debug("doigt 3 ouvert / 4 fermé\n  " );
    doigt_4.set_target_to_open() ;
    doigt_5.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    event_flag.set (  FLAG_START_DOIGT_4 | FLAG_START_DOIGT_5);     // démarre les moteurs
    event_flag.wait_all(   FLAG_STOP_DOIGT_4 | FLAG_STOP_DOIGT_5); // attend que les moteurs
 print_debug("doigt 4 ouvert / 5 fermé\n  " );
    doigt_5.set_target_to_open() ; 
    event_flag.set ( FLAG_START_DOIGT_5);     // démarre les moteurs
    event_flag.wait_all(  FLAG_STOP_DOIGT_5); // attend que les moteurs
  print_debug("tout ouvert\n  " );   
   }
}
void fuck()
{ 
//il faut que l'init soit faite et que le bras soit à l'init
   if (init_position_done ==1 && current_position == 0 ) {
   open_hand(); 
  // move_position_init(); //INIT au cas ou on  a bougé
     doigt_0.set_target_to_close_to_endstop(); //on ferme la pince
    event_flag.set(FLAG_START_DOIGT_0); // démarre les moteurs 
    event_flag.wait_all(FLAG_STOP_DOIGT_0); // attend que les moteurs
    print_debug("la pince est fermee\n  " );
     // on définit la nouvelle cible
    doigt_1.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_2.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    
    doigt_4.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_5.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    print_debug("on ferme et stop\n" );
    event_flag.set(FLAG_START_DOIGT_1 | FLAG_START_DOIGT_2 |     FLAG_START_DOIGT_4 | FLAG_START_DOIGT_5);     // démarre les moteurs
    move_position_fuck();
    event_flag.wait_all(FLAG_STOP_DOIGT_1 | FLAG_STOP_DOIGT_2 |    FLAG_STOP_DOIGT_4 | FLAG_STOP_DOIGT_5); // attend que les moteurs
     
       }
}
void thumb_down()
{  
   //il faut que l'init soit faite et que le bras soit à l'init
   //il faut que l'init soit faite et que le bras soit à l'init
   if (init_position_done ==1 && current_position == 0 ) {
   open_hand();  
   //move_position_init(); //INIT au cas ou on  a bougé
     // on définit la nouvelle cible 
    doigt_2.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_3.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_4.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    doigt_5.set_target_to_close_and_stop() ;// on ferme le doigt et stop
    print_debug("on ferme et stop\n" );
    event_flag.set (FLAG_START_DOIGT_2 | FLAG_START_DOIGT_3 | FLAG_START_DOIGT_4 | FLAG_START_DOIGT_5);     // démarre les moteurs
    move_position_thumb_down(); 
    event_flag.wait_all(  FLAG_STOP_DOIGT_2 | FLAG_STOP_DOIGT_3 | FLAG_STOP_DOIGT_4 | FLAG_STOP_DOIGT_5); // attend que les moteurs
   
   }
}

//commande de position
void init_vers_verre(){ 
   // | ep plat | ep haut | coude | poig | poig haut | flag_enable_PID
   //on verifie que le poignet haut est bon

    while (end_stop.read() == 0 ) {
    move_arm ((int) motor_epaule_a_plat.get_angle() +5,
              (int) motor_epaule_haut.get_angle() +5,
              (int) motor_coude.get_angle() +5,
              (int) motor_poignet.get_angle() +5,
              (int) motor_poignet_haut.get_angle() +5,
              (int) 10);  
              
  }
  // init motor_poignet_haut
  motor_poignet_haut.init_position(); 
  last_target_poignet_haut = 0 ;
open_hand();
move_position_init(); //INIT au cas ou on  a bougé
fumee_lumieres();//fumée lumières

move_position_hors_carrousel();  //tourne hors du carrousel  
move_position_avant_verre();  //position avant verre
close_thumb();//ferme le pouce
move_position_verre(); // position verre
close_hand(); //prend le verre

          current_position=1;
}
void verre_vers_init(){ 
//move_position_verre(); // position verre
move_position_verre();
 open_hand_without_thumb();
move_position_avant_verre();  //position avant verre  
move_position_hors_carrousel();  //tourne hors du carrousel 
 open_thumb() ;   //ouvre le pouce
 fumee_lumieres();//fumée lumières

 move_position_init(); //INIT
 open_hand(); //ouvre la main
  current_position=0;  
} 
void verre_vers_pos1(){
move_position_verre(); // position verre
move_position_avant_verre();  //position avant verre 
move_position_1(); // POS1  
   current_position=2;
}
void verre_vers_pos2(){
move_position_verre(); // position verre
move_position_avant_verre();  //position avant verre
move_position_hors_carrousel();  //tourne hors du carrousel 
   move_position_2(); // POS2
 current_position=3; //
}
void verre_vers_pos3(){
move_position_verre(); // position verre
move_position_avant_verre();  //position avant verre
 
   move_position_3(); // POS3
   current_position=4; //
}
void pos23_vers_verre(){
move_position_hors_carrousel();  //tourne hors du carrousel 
move_position_avant_verre();  //position avant verre
move_position_verre(); // position verre
  current_position=1;
}
void pos1_vers_verre(){
move_position_1();  
move_position_avant_verre();  //position avant verre
move_position_verre(); // position verre
  current_position=1;
}
void pos13_vers_pos2(){
move_position_hors_carrousel();  //tourne hors du carrousel 
   move_position_2(); // POS2
   current_position=3;
}
void pos12_vers_pos3(){
move_position_hors_carrousel();  //tourne hors du carrousel 
move_position_hors_carrousel();  //tourne hors du carrousel 
 move_position_3(); // POS3
   current_position=4; //
}

void pos23_vers_pos1(){
move_position_hors_carrousel();  //tourne hors du carrousel 
  move_position_1(); // POS1  
   current_position=2;
}


/*pour régler les positions du bras en debug*/
void reglage_position_bras() {
  if (reglage_bras_current_position == 0) { 
      reglage_bras_current_position = reglage_bras_current_position +1;
      move_position_hors_carrousel();  //tourne hors du carrousel 
      return;
      }
   if (reglage_bras_current_position == 1) { 
      reglage_bras_current_position = reglage_bras_current_position +1;
      move_position_avant_verre();  //position avant verre
      return;
      }      
if (reglage_bras_current_position == 2) { 
      reglage_bras_current_position = reglage_bras_current_position +1;
      close_thumb();//ferme le pouce
      move_position_verre(); // position verre
      return;
      }    
if (reglage_bras_current_position == 3) { 
      reglage_bras_current_position = reglage_bras_current_position +1;
      close_hand(); //prend le verre
      return;
      } 
if (reglage_bras_current_position == 4) { 
      reglage_bras_current_position = reglage_bras_current_position +1;
      move_position_avant_verre();  //position avant verre 
      move_position_1(); // POS1  
      return;
      } 
if (reglage_bras_current_position == 5) { 
      reglage_bras_current_position = reglage_bras_current_position +1;
      move_position_hors_carrousel();  //tourne hors du carrousel 
      move_position_2(); // POS2
      return;
      }    
if (reglage_bras_current_position == 6) { 
      reglage_bras_current_position = reglage_bras_current_position +1;
      move_position_hors_carrousel();  //tourne hors du carrousel 
       move_position_3(); // POS3
       return;
      }   
  if (reglage_bras_current_position == 7) { 
      reglage_bras_current_position = reglage_bras_current_position +1;
      move_position_hors_carrousel();  //tourne hors du carrousel 
      move_position_avant_verre();  //position avant verre
      move_position_verre(); // position verre
      return;
      }        
  if (reglage_bras_current_position == 8) { 
      reglage_bras_current_position = 0;
      
      open_hand_without_thumb();
      move_position_avant_verre();  //position avant verre  
      move_position_hors_carrousel();  //tourne hors du carrousel 
      open_thumb() ;   //ouvre le pouce
      move_position_init(); //INIT
      open_hand(); //ouvre la main
      return;
      }        
}


 
//  position  0: position_init
 // position  1: prend_le_verre 
 // position  2: carrousel1 
 // position  3: carrousel2 
 // position  4: carrousel3 
 // position  5: lache_le_verre


void position_init(){
  print_debug("position_init , current_position = % \n",current_position); 
  if (current_position != 0 ) {
    if (current_position == 1){verre_vers_init();} // verre -> init
    if (current_position == 2){ pos1_vers_verre();// pos1 -> init
                                verre_vers_init();} 
    
    if (current_position == 3){ pos23_vers_verre();// pos2 -> init
                                verre_vers_init();} 
    if (current_position == 4){ pos23_vers_verre();// pos3 -> init
                                verre_vers_init();}                             
   }
}
void prend_le_verre(){
  print_debug("position prend_le_verre , current_position = %i \n",current_position); 
  if (current_position != 1 ) {
    if (current_position == 0){init_vers_verre();} //init -> verre
    if (current_position == 2){pos1_vers_verre();} //position1 -> verre
    if (current_position == 3){pos23_vers_verre();} //position2 -> verre
    if (current_position == 4){pos23_vers_verre();} //position3 -> verre
   }
}
void position1(){
  print_debug("position 1 , current_position = %i \n",current_position); 
  if (current_position != 2 /*position 2: carrousel1*/ ) {
    if (current_position == 0){init_vers_verre(); //init -> pos1
                              verre_vers_pos1();}
    
   if (current_position == 1){verre_vers_pos1();} //verre -> pos1
   if (current_position == 3){pos23_vers_pos1();} //pos2 -> pos1
   if (current_position == 4){pos23_vers_pos1();} //pos3 -> pos1
   }
}
void position2(){
  print_debug("position 2 , current_position = %i \n",current_position); 
 if (current_position != 3 ) {
    if (current_position == 0){init_vers_verre(); //init -> pos2
                              verre_vers_pos2(); 
                              }
    
   if (current_position == 1){verre_vers_pos2();} //verre -> pos2
   if (current_position == 2){pos13_vers_pos2();} //pos1 -> pos2
   if (current_position == 4){pos13_vers_pos2();} //pos3 -> pos2
   }

}
void position3(){
  print_debug("position 3, current_position = %i \n",current_position); 
   if (current_position != 4 ) {
    if (current_position == 0){init_vers_verre(); //init -> pos3
                              verre_vers_pos3(); 
                              }
    
   if (current_position == 1){verre_vers_pos3();} //verre -> pos3
   if (current_position == 2){pos12_vers_pos3();} //pos1 -> pos3
   if (current_position == 3){pos12_vers_pos3();} //pos2 -> pos3
   }
}
void lache_le_verre(){}
//Commandes manuelles
//forward -> superieur à 0, backward -> inférieur à 0  
/***epaule à plat***/
void epaule_a_plat_run_forward(){  
                                motor_epaule_a_plat.speed_manual_run = 800; //flag manuel
                      event_flag.set(FLAG_START_EPAULE_A_PLAT);} //demarre le moteur
                      
void epaule_a_plat_run_backward(){  
                                 motor_epaule_a_plat.speed_manual_run = -800; 
                      event_flag.set(FLAG_START_EPAULE_A_PLAT); //demarre le moteur
                      }
void epaule_a_plat_stop(){ 
                    motor_epaule_a_plat.motor_stop() ;
                    // print_debug("move_arm ( %f , \t  %f , \t %f , \t %f , \t %f , 0 ); \n ",  motor_epaule_a_plat.get_angle()  ,  motor_epaule_haut.get_angle()  ,  motor_coude.get_angle()  ,  motor_poignet.get_angle()  ,  motor_poignet_haut.get_angle() );
                    // print_debug("move_arm ( %i , \t  %i , \t %i , \t %i , \t %i , 0 ); \n ", (int)  motor_epaule_a_plat.get_angle()  ,  (int)  motor_epaule_haut.get_angle()  ,  (int)  motor_coude.get_angle()  ,  (int)  motor_poignet.get_angle()  ,  (int)  motor_poignet_haut.get_angle() );
                    print_debug("move_arm ( %i ,   %i ,  %i ,  %i ,  %i , 0 ); \n ",  (int) round (motor_epaule_a_plat.get_angle())  , (int)  round (motor_epaule_haut.get_angle())  ,  (int)  round (motor_coude.get_angle())  , (int)  round (motor_poignet.get_angle())  ,  (int) round (motor_poignet_haut.get_angle() ));
                        }

/***epaule haut***/
void epaule_haut_run_forward(){  
                                motor_epaule_haut.speed_manual_run = 1000; //flag manuel
                      event_flag.set(FLAG_START_EPAULE_HAUT); //demarre le moteur
                      }
void epaule_haut_run_backward(){  
                                motor_epaule_haut.speed_manual_run = -1000;//flag manuel 
                      event_flag.set(FLAG_START_EPAULE_HAUT); //demarre le moteur
                      }
void epaule_haut_stop(){ 
                      motor_epaule_haut.motor_stop();
                     print_debug("move_arm ( %i ,   %i ,  %i ,  %i ,  %i , 0 ); \n ",  (int) round (motor_epaule_a_plat.get_angle())  , (int)  round (motor_epaule_haut.get_angle())  ,  (int)  round (motor_coude.get_angle())  , (int)  round (motor_poignet.get_angle())  ,  (int) round (motor_poignet_haut.get_angle() ));
                      
                    //   print_debug("motor_epaule_a_plat : angle %f \n",  motor_epaule_a_plat.get_angle() );
                    // print_debug("motor_epaule_haut   :  angle %f  \n",  motor_epaule_haut.get_angle() );
                    // print_debug("motor_coude         : angle %f \n",   motor_coude.get_angle() );
                    // print_debug("motor_poignet       : angle %f \n",  motor_poignet.get_angle() );
                    // print_debug("motor_poignet_haut  : angle %f  \n",   motor_poignet_haut.get_angle() );
                      }

/***coude***/
void coude_run_forward(){   
                          motor_coude.speed_manual_run = 1000; //flag manuel
                      event_flag.set(FLAG_START_COUDE); //demarre le moteur
                      }
void coude_run_backward(){ 
                          motor_coude.speed_manual_run = -1000;//flag manuel 
                      event_flag.set(FLAG_START_COUDE); //demarre le moteur
                      }
void coude_stop(){        
                    motor_coude.motor_stop();
                    print_debug("move_arm ( %i ,   %i ,  %i ,  %i ,  %i , 0 ); \n ",  (int) round (motor_epaule_a_plat.get_angle())  , (int)  round (motor_epaule_haut.get_angle())  ,  (int)  round (motor_coude.get_angle())  , (int)  round (motor_poignet.get_angle())  ,  (int) round (motor_poignet_haut.get_angle() ));
                     
                    // print_debug("motor_epaule_a_plat : angle %f \n",  motor_epaule_a_plat.get_angle() );
                    // print_debug("motor_epaule_haut   : angle %f  \n",  motor_epaule_haut.get_angle() );
                    // print_debug("motor_coude         : angle %f \n",   motor_coude.get_angle() );
                    // print_debug("motor_poignet       : angle %f \n",  motor_poignet.get_angle() );
                    // print_debug("motor_poignet_haut  : angle %f  \n",   motor_poignet_haut.get_angle() );
                    }

/***poignet***/
void poignet_run_forward(){ 
                         
                       motor_poignet.speed_manual_run = 3500; //flag manuel
                       event_flag.set(FLAG_START_POIGNET); //demarre le moteur
                      //print_debug("forward  speed_manual_run_forward = %i speed_manual_run_backward: %i stop: %i \n", motor_poignet.speed_manual_run_forward, motor_poignet.speed_manual_run_backward,motor_poignet.flag_manual_stop);

                      }
void poignet_run_backward(){ 
                       
                      motor_poignet.speed_manual_run = -3500;//flag manuel 
                      event_flag.set(FLAG_START_POIGNET); //demarre le moteur
                      //print_debug("backward  speed_manual_run_forward = %i speed_manual_run_backward: %i stop: %i \n", motor_poignet.speed_manual_run_forward, motor_poignet.speed_manual_run_backward,motor_poignet.flag_manual_stop);

                      }
void poignet_stop(){         
                    motor_poignet.motor_stop();
                    print_debug("move_arm ( %i ,   %i ,  %i ,  %i ,  %i , 0 ); \n ",  (int) round (motor_epaule_a_plat.get_angle())  , (int)  round (motor_epaule_haut.get_angle())  ,  (int)  round (motor_coude.get_angle())  , (int)  round (motor_poignet.get_angle())  ,  (int) round (motor_poignet_haut.get_angle() ));
                     
                    // print_debug("motor_epaule_a_plat : angle %f \n",  motor_epaule_a_plat.get_angle() );
                    // print_debug("motor_epaule_haut   :  angle %f  \n",  motor_epaule_haut.get_angle() );
                    // print_debug("motor_coude         : angle %f \n",   motor_coude.get_angle() );
                    // print_debug("motor_poignet       : angle %f \n",  motor_poignet.get_angle() );
                    // print_debug("motor_poignet_haut  : angle %f  \n",   motor_poignet_haut.get_angle() );
                       }                               

/***poignet haut***/
void poignet_haut_run_forward(){    
                                  motor_poignet_haut.speed_manual_run = 1500; //flag manuel
                      event_flag.set(FLAG_START_POIGNET_HAUT); //demarre le moteur
                      }
void poignet_haut_run_backward(){  
                                  motor_poignet_haut.speed_manual_run = -1500;//flag manuel 
                      event_flag.set(FLAG_START_POIGNET_HAUT); //demarre le moteur
                      }
void poignet_haut_stop(){         
                    motor_poignet_haut.motor_stop();
                    print_debug("move_arm ( %i ,   %i ,  %i ,  %i ,  %i , 0 ); \n ",  (int) round (motor_epaule_a_plat.get_angle())  , (int)  round (motor_epaule_haut.get_angle())  ,  (int)  round (motor_coude.get_angle())  , (int)  round (motor_poignet.get_angle())  ,  (int) round (motor_poignet_haut.get_angle() ));
                     
                    //   print_debug("motor_epaule_a_plat : angle %f \n",  motor_epaule_a_plat.get_angle() );
                    // print_debug("motor_epaule_haut   :  angle %f  \n",  motor_epaule_haut.get_angle() );
                    // print_debug("motor_coude         : angle %f \n",   motor_coude.get_angle() );
                    // print_debug("motor_poignet       : angle %f \n",  motor_poignet.get_angle() );
                    // print_debug("motor_poignet_haut  : angle %f  \n",   motor_poignet_haut.get_angle() );
                         }


void doigt_ouvre( int num_motor  ) {
    if (num_motor == 0){doigt_0.motor_open();}
    if (num_motor == 1){doigt_1.motor_open();}
    if (num_motor == 2){doigt_2.motor_open();}
    if (num_motor == 3){doigt_3.motor_open();}
    if (num_motor == 4){doigt_4.motor_open();} 
    if (num_motor == 5){doigt_5.motor_open();}
};
void doigt_ferme( int num_motor  ){
    if (num_motor == 0){doigt_0.motor_close();}
    if (num_motor == 1){doigt_1.motor_close();}
    if (num_motor == 2){doigt_2.motor_close();}
    if (num_motor == 3){doigt_3.motor_close();}
    if (num_motor == 4){doigt_4.motor_close();} 
    if (num_motor == 5){doigt_5.motor_close();}
};
void doigt_stop( int num_motor  ){
    if (num_motor == 0){doigt_0.motor_stop();}
    if (num_motor == 1){doigt_1.motor_stop();}
    if (num_motor == 2){doigt_2.motor_stop();}
    if (num_motor == 3){doigt_3.motor_stop();}
    if (num_motor == 4){doigt_4.motor_stop();} 
    if (num_motor == 5){doigt_5.motor_stop();}
};

//###########################
//           THREAD
//##########################
//Threads moteur
Thread thread_motor_coude(osPriorityNormal,2000, nullptr,  nullptr);
Thread thread_motor_poignet(osPriorityNormal,2000, nullptr,  nullptr);
Thread thread_motor_epaule_a_plat(osPriorityNormal,2000, nullptr,  nullptr);
Thread thread_motor_epaule_haut(osPriorityNormal,2000, nullptr,  nullptr);
Thread thread_motor_poignet_haut(osPriorityNormal,2000, nullptr,  nullptr);

//Threads moteur main
Thread thread_motor_doigt0(osPriorityNormal,1000, nullptr,  nullptr);
Thread thread_motor_doigt1(osPriorityNormal,1000, nullptr,  nullptr);
Thread thread_motor_doigt2(osPriorityNormal,1000, nullptr,  nullptr);
Thread thread_motor_doigt3(osPriorityNormal,1000, nullptr,  nullptr);
Thread thread_motor_doigt4(osPriorityNormal,1000, nullptr,  nullptr);
Thread thread_motor_doigt5(osPriorityNormal,1000, nullptr,  nullptr);

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

Mail<mail_t, 255> mail_box;

void run_motor_in_thread(mbed_hall_driven_motor *motor)
{
  while (true)
  {
    event_flag.wait_all(motor->_flag_start); // attend le flag de démarrage 
    //pour arreter le moteur en butée avec la commande manuelle,
    //on fait un boucle et on sort quand quand la vitesse est à 0 
    //print_debug("thread angle %s : %f\n",motor->_motor_name.c_str(), motor->get_angle());
    while(true)
            {
              motor->run();

              if (motor->speed_manual_run == 0  ) {break;}
             }
    //le flag flag_manual_stop stop le thread 
    //motor->motor_stop(); 
    // print_debug("thread angle %s : %f\n",motor->_motor_name.c_str(), motor->get_angle());
    event_flag.set(motor->_flag_stop);  
     //print_debug("stop in thread speed_manual_run_forward = %i speed_manual_run_backward: %i stop: %i \n", motor->speed_manual_run_forward, motor->speed_manual_run_backward,motor->flag_manual_stop);

  }
  //print_debug(" fin du thread! \n");
}

void run_motor_doigt_in_thread(mbed_current_driven_motor *motor)
{
  while (true)
  {
    event_flag.wait_all(motor->_flag_start); // attend le flag de démarrage 
    // print_debug("start motor\n");
    //  motor->displayName();
    motor->run();
    event_flag.set(motor->_flag_stop);
    ; // attend que le moteur renvoie le flag de stop
    //  print_debug("stop motor %c \n",(char) motor->get_motor_name());
  }
}

//###########################
//           INIT Position
//##########################

int init_position(){

 // si le flag stop est 1 on bouge de 5 degrés pour avoir flag stop à 0
                             while (end_stop.read() == 0 ) {
                              move_arm ((int) motor_epaule_a_plat.get_angle() +5,
                                        (int) motor_epaule_haut.get_angle() +5,
                                        (int) motor_coude.get_angle() +5,
                                        (int) motor_poignet.get_angle() +5,
                                        (int) motor_poignet_haut.get_angle() +5,
                                        (int) 10);  
                                       
                            }
                            // init coude
                            motor_coude.init_position();
                            // init epaule_haut
                            motor_epaule_haut.init_position();
                            // init motor_poignet_haut
                            motor_poignet_haut.init_position(); 
                            // init poignet
                            motor_poignet.init_position();
                            //init position moteurs
                            // last_target_epaule_a_plat = 0 ;
                            last_target_epaule_haut = 0 ;
                            last_target_coude = 0 ;
                            last_target_poignet = 0 ;
                            last_target_poignet_haut = 0 ;
                            // print_debug("angle poignet1: %f\n", motor_poignet.get_angle());
                          // init epaule_a_plat
                          //on met la main dans la bonne position
                          //motor_poignet._debug_flag =true;
                          //  move_arm (motor_epaule_a_plat.get_angle() , //motor_epaule_a_plat
                          //               0, //motor_epaule_haut
                          //               0, //motor_coude
                          //               166, //motor_poignet
                          //               0,//motor_poignet_haut
                          //               10);  //pEnable_PID
                              // print_debug("angle poignet2: %f\n", motor_poignet.get_angle());
                            motor_epaule_a_plat.init_position();
                            //init position moteurs
                            last_target_epaule_a_plat = 0 ;
                            //  print_debug("angle poignet3: %f\n", motor_poignet.get_angle());
                            // move_arm (0, //motor_epaule_a_plat
                            //             0, //motor_epaule_haut
                            //             0, //motor_coude
                            //             166, //motor_poignet
                            //             0, //motor_poignet_haut
                            //             10); //pEnable_PID
                              // print_debug("angle poignet4: %f\n", motor_poignet.get_angle());
                            //ouvre la main
                            open_hand();

     return 1; //l'init position a été faite                     
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
ID = 5 -> boutons

cmde = 1 -> move_arm (parametres: angles = DATA[1..5]) 
cmde = 2 -> open_hand
cmde = 3 -> open_hand_without_thumb
cmde = 4 -> close_thumb
cmde = 5 -> close_hand
cmde = 6 -> hand_ILY_from_open
cmde = 7 -> thumb_up
cmde = 8 -> thumb_down
cmde = 9 -> bouge les doigts
cmde = 10 -> fuck

 
cmde = 12 -> init_position

//commandes manuelles du bras (manettes)

 cmde == 20 epaule_a_plat_run_forward(); }
 cmde == 21 epaule_a_plat_run_backward(); }
 cmde == 22 epaule_a_plat_stop(); }
 cmde == 23 epaule_haut_run_forward(); }
 cmde == 24 epaule_haut_run_backward(); }
 cmde == 25 epaule_haut_stop(); }
 cmde == 26 coude_run_forward(); }
 cmde == 27 coude_run_backward(); }
 cmde == 28 coude_stop(); }    
 cmde == 29 poignet_run_forward(); }
 cmde == 30 poignet_run_backward(); }
 cmde == 31 poignet_stop(); }   
 cmde == 32 poignet_haut_run_forward(); }
 cmde == 33 poignet_haut_run_backward(); }
 cmde == 34 poignet_haut_stop(); }  


//commandes manuelles des doigts

cmde == 40  doigt_ouvre((int)(mail->data_1)); } 
cmde == 41  doigt_ferme((int)(mail->data_1)); } 
cmde == 42  doigt_stop((int)(mail->data_1)); } 

// donne les positions
cmde = 100 -> donne la position bras/ renvoie les angles = data_3..8  (parametres: destinataire data_1  )
cmde = 101 -> donne la position des doigts sur 6 bits
 // commande de position
cmde == 120 position_init 
cmde == 121 prend_le_verre 
cmde == 122 position1 
cmde == 123 position2 
cmde == 124 position3 
cmde == 125 lache_le_verre 

cmde == 126 reglage_position_bras : pour régler les positions du bras en debug, on parcours toutes les positions

cmde == 150 sert le cocktail 
          (n°destinataire = 4 (bras), n° emetteur , cmde=150, n° bouteille, temps dose,n° bouteille, temps dose,n° bouteille, temps dose)
          bouteilles = 1..16
          19 : fin du cocktail, lache le verre et retour à l'init 


//déclenchement des commandes
cmde = 200 -> appelle la fonction run pour executer toutes les commandes de la mailbox
cmde = 201 -> envoi run sur les autres noeuds -> send_buffer (mail->data_1,DEVICE_ID,200,0,0,0,0,0,0);  
********/

void run_bras()
{
  while (!mail_box.empty())
  {
    mail_t *mail = mail_box.try_get();

    if (mail != nullptr)
    { print_debug("mail get cmde %i \n", mail->cmde);
      if (mail->cmde == 1 && init_position_done ==1 /*on bouge uniquement si l'init a été fait*/)
                                                
      { move_arm ((int)(mail->data_1*360.0/255.0),
                  (int)(mail->data_2*360.0/255.0),
                  (int)(mail->data_3*360.0/255.0),
                  (int)(mail->data_4*360.0/255.0),
                  (int)(mail->data_5*360.0/255.0),
                  (int)(mail->data_6));
        print_debug(" position bras %i, %i, %i, %i, %i, %i \n", mail->data_1,mail->data_2,mail->data_3,mail->data_4,mail->data_5,mail->data_6);}
      if (mail->cmde == 1 && init_position_done ==0 /*l'init n'a pas été fait*/)  {
              print_debug("l'init n'a pas ete fait! \n");
              print_debug("position demande %i, %i, %i, %i, %i, %i \n", mail->data_1,mail->data_2,mail->data_3,mail->data_4,mail->data_5,mail->data_6);}
      if (mail->cmde == 2){open_hand();}
      if (mail->cmde == 3){open_hand_without_thumb();}
      if (mail->cmde == 4){close_thumb();}
      if (mail->cmde == 5){close_hand();}
      if (mail->cmde == 6){hand_ILY_from_open();}
      if (mail->cmde == 7){thumb_up();}
      if (mail->cmde == 8){thumb_down();}
      if (mail->cmde == 9){bouge_les_doigts();}
      if (mail->cmde == 10){fuck();}
      // if (mail->cmde == 7){motor_epaule_a_plat.init();}
      // if (mail->cmde == 8){motor_epaule_haut.init();}
      // if (mail->cmde == 9){motor_coude.init();}
      // if (mail->cmde == 10){motor_poignet.init();}
      // if (mail->cmde == 11){motor_poignet_haut.init();}
      if (mail->cmde == 12){init_position_done = init_position(); }
      //commandes manuelles du bras (manettes)
      if (mail->cmde == 20){epaule_a_plat_run_forward(); }
      if (mail->cmde == 21){epaule_a_plat_run_backward(); }
      if (mail->cmde == 22){epaule_a_plat_stop(); }
      if (mail->cmde == 23){epaule_haut_run_forward(); }
      if (mail->cmde == 24){epaule_haut_run_backward(); }
      if (mail->cmde == 25){epaule_haut_stop(); }
      if (mail->cmde == 26){coude_run_forward(); }
      if (mail->cmde == 27){coude_run_backward(); }
      if (mail->cmde == 28){coude_stop(); }    
      if (mail->cmde == 29){poignet_run_forward(); }
      if (mail->cmde == 30){poignet_run_backward(); }
      if (mail->cmde == 31){poignet_stop(); }   
      if (mail->cmde == 32){poignet_haut_run_forward(); }
      if (mail->cmde == 33){poignet_haut_run_backward(); }
      if (mail->cmde == 34){poignet_haut_stop(); } 
      //commandes manuelles des doigts
      if (mail->cmde == 40){doigt_ouvre((int)(mail->data_1)); } 
      if (mail->cmde == 41){doigt_ferme((int)(mail->data_1)); } 
      if (mail->cmde == 42){doigt_stop((int)(mail->data_1)); } 
      // commande de position 
      /*on bouge uniquement si l'init a été fait*/ 
            
      if (mail->cmde == 120 && init_position_done ==1 ){position_init(); }
      if (mail->cmde == 121 && init_position_done ==1 ){prend_le_verre(); }
      if (mail->cmde == 122 && init_position_done ==1 ){fumee_lumiere_on = 0 ; position1(); }
      if (mail->cmde == 123 && init_position_done ==1 ){fumee_lumiere_on = 0 ;position2(); }
      if (mail->cmde == 124 && init_position_done ==1 ){fumee_lumiere_on = 0 ;position3(); }
      if (mail->cmde == 125 && init_position_done ==1 ){lache_le_verre(); }
      /*pour régler les positions du bras en debug*/
      if (mail->cmde == 126 && init_position_done ==1 ){reglage_position_bras(); } 

      /* cmde == 150 sert le cocktail 
                (n°destinataire = 4 (bras), n° emetteur , cmde=150, n° bouteille, temps dose,n° bouteille, temps dose,n° bouteille, temps dose)
                bouteilles = 1..18
                19 : fin du cocktail, lache le verre et retour à l'init */
      if (mail->cmde == 150 && init_position_done ==0){
                        print_debug(" l'init n'a pas ete fait \n");
                        print_debug("position demande %i, %i, %i, %i, %i, %i \n", mail->data_1,mail->data_2,mail->data_3,mail->data_4,mail->data_5,mail->data_6);}
      if (mail->cmde == 150 && init_position_done ==1)
          {
            fumee_lumiere_on = 1 ;
            if ((int)(mail->data_1)>= 1 && (int)(mail->data_1)<= 6)
            {position1(); 
              send_buffer( 1, DEVICE_ID , 1, (int)(mail->data_1),  0 ,   0,  0,  0,   0 ); /*positionne le carrousel*/
              send_buffer( 1, DEVICE_ID ,  3, (int)(mail->data_2),   0,  0,  0,   0,   0 ); /*envoi servir sur carrousel 1*/ 
              send_buffer( 1, DEVICE_ID ,  201,  4 ,   0,  0,  0,   0,   0 ); /*à la fin du service, envoi run sur le bras*/
              send_buffer (1, DEVICE_ID,200,0,0,0,0,0,0); /*envoi run sur le carrousel*/ 
            }
            else if ((int)(mail->data_1)>= 7 && (int)(mail->data_1)<= 12)
            {position2();
              send_buffer( 2, DEVICE_ID , 1, (int)(mail->data_1) - 6,  0 ,   0,  0,  0,   0 ); /*positionne le carrousel*/
              send_buffer( 2, DEVICE_ID ,  3, (int)(mail->data_2),   0,  0,  0,   0,   0 ); /*envoi servir sur carrousel 2*/ 
              send_buffer( 2, DEVICE_ID ,  201,  4 ,   0,  0,  0,   0,   0 ); /*à la fin du service, envoi run sur le bras*/
              send_buffer (2, DEVICE_ID,200,0,0,0,0,0,0); /*envoi run sur le carrousel*/ 
            }
            else if ((int)(mail->data_1)>= 13 && (int)(mail->data_1)<= 18)
            {position3();
              send_buffer( 3, DEVICE_ID , 1, (int)(mail->data_1) - 12,  0 ,   0,  0,  0,   0 ); /*positionne le carrousel*/
              send_buffer( 3, DEVICE_ID ,  3, (int)(mail->data_2),   0,  0,  0,   0,   0 ); /*envoi servir sur carrousel 3*/ 
              send_buffer( 3, DEVICE_ID ,  201,  4 ,   0,  0,  0,   0,   0 ); /*à la fin du service, envoi run sur le bras*/
              send_buffer (3, DEVICE_ID,200,0,0,0,0,0,0); /*envoi run sur le carrousel*/ 
            }
            else if ((int)(mail->data_1)>= 19) /* retour à l'init*/
            {position_init(); 
            }
            print_debug("free(mail) \n");
            mail_box.free(mail);
            return; /*on sort de la pile de mail*/
          }
           
                
     
      if (mail->cmde == 201){  send_buffer (mail->data_1,DEVICE_ID,200,0,0,0,0,0,0); /*envoi run sur les autres noeuds*/
                              print_debug("free(mail) \n");
                              mail_box.free(mail);
                              return; /*on sort de la pile de mail*/
                            } 
         }      
           print_debug("free(mail) \n");
           mail_box.free(mail);
   

    // ThisThread::sleep_for(chrono::milliseconds(1s));
  } 
}


//###########################
//           INIT cartes
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

//debug
//motor_coude._debug_flag = true;

   // init coude moteurs
  motor_coude.init();
  // init epaule_haut
  motor_epaule_haut.init();
  // init epaule_a_plat
  motor_poignet_haut.init(); 
  // init poignet
  motor_poignet.init();
// init epaule_a_plat
  motor_epaule_a_plat.init();
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

  //motor_poignet._debug_flag = true  ;
}

