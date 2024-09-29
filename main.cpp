
#include "config_fonctions.hpp"
#include "debug_print.h" // Inclure le fichier 

//###########################
//           MAIN
//##########################
int main()
{

   // initialisation
  print_debug("init all\n");
  init_all();
  
 // Initialiser la gestion des messages de débogage dans un thread
    print_debug_in_thread(true);   

 print_debug("start ecoute CAN \n");
   CANMessage msg;
  can.filter (DEVICE_ID,0xFFFFFF,CANStandard,0);
  while (true)
  {
     if (can.read(msg)) 
          {  
            print_debug("recu  :id: \t %i data: \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n", msg.id, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);             
            if (mail_box.full())  {   print_debug("error mail_box.full\n" ); }
            else if (msg.data[1] == 100)
                    {//cmde = 100 -> donne la position bras/ renvoie les angles = data_2..7  (parametres: destinataire data_1  )
                      send_buffer (    msg.data[0]  
                                      , DEVICE_ID 
                                      , motor_epaule_a_plat.get_angle()*255.0/360.0
                                      , motor_epaule_haut.get_angle()*255.0/360.0
                                      , motor_coude.get_angle()*255.0/360.0
                                      , motor_poignet.get_angle()*255.0/360.0
                                      , motor_poignet_haut.get_angle() *255.0/360.0
                                      , 0
                                      ,0
                                    );
                    
                    //  print_debug("envoye  :id: %i data: \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n",msg.data[0], ack.data[0], ack.data[1], ack.data[2], ack.data[3], ack.data[4], ack.data[5], ack.data[6], ack.data[7]);
                    }
              /*101 -> donne la position main*/
              else if (msg.data[1] == 101)
                    {
                      // Envoyer l'accusé de réception avec la position des doigts  
                      send_buffer ( msg.data[0]
                                    , DEVICE_ID 
                                    , doigt_0.get_position()
                                    , doigt_1.get_position()
                                    , doigt_2.get_position()
                                    , doigt_3.get_position()
                                    , doigt_4.get_position()
                                    , doigt_5.get_position()
                                    ,0
                                      )  ; 
                    }
              /* run */
              else  if (msg.data[1] == 200)   { run_bras(); } 
              else  if (msg.data[1] == 150)   { 
                    /*cmde == 150 sert le cocktail 
                    pour les cocktails, on peut mettre 3 actions dans le message */ 
                    if (msg.data[1] == 150 && msg.data[2]!=0){
                                       mail_t *mail = mail_box.try_alloc();
                                       mail->cmde = msg.data[1];
                                       mail->data_1 = msg.data[2];
                                       mail->data_2 = msg.data[3];
                                       mail->data_3 = 0;
                                       mail->data_4 = 0;
                                       mail->data_5 = 0;
                                       mail->data_6 = 0; 
                                       mail_box.put(mail);}
                     if (msg.data[1] == 150 && msg.data[4]!=0){
                                       mail_t *mail = mail_box.try_alloc();
                                       mail->cmde = msg.data[1];
                                       mail->data_1 = msg.data[4];
                                       mail->data_2 = msg.data[5];
                                       mail->data_3 = 0;
                                       mail->data_4 = 0;
                                       mail->data_5 = 0;
                                       mail->data_6 = 0; 
                                       mail_box.put(mail);}
                     if(msg.data[1] == 150 && msg.data[6]!=0){
                                       mail_t *mail = mail_box.try_alloc();
                                       mail->cmde = msg.data[1];
                                       mail->data_1 = msg.data[6];
                                       mail->data_2 = msg.data[7];
                                       mail->data_3 = 0;
                                       mail->data_4 = 0;
                                       mail->data_5 = 0;
                                       mail->data_6 = 0; 
                                       mail_box.put(mail);}
               } 
              else { 
                       //stocke les parametres dans un mail
                     mail_t *mail = mail_box.try_alloc();
                     mail->cmde = msg.data[1];
                     mail->data_1 = msg.data[2];
                     mail->data_2 = msg.data[3];
                     mail->data_3 = msg.data[4];
                     mail->data_4 = msg.data[5];
                     mail->data_5 = msg.data[6];
                     mail->data_6 = msg.data[7]; 
                     mail_box.put(mail);
                     
                 }
            
          }

  }
}
