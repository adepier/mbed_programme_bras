#include "config_motor.hpp"
 
 
 

//###########################
//           MAIN
//##########################
int main()
{

   // initialisation
  printf("init all\n");
  init_all();
 printf("start montée \n   ");
   

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
