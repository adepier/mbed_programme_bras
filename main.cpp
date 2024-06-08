
#include "config_fonctions.hpp"


//###########################
//           MAIN
//##########################
int main()
{

   // initialisation
  printf("init all\n");
  init_all();
  // init_position();

 printf("start ecoute CAN \n");
   CANMessage msg;
  can.filter (DEVICE_ID,0xFFFFFF,CANStandard,0);
  while (true)
  {
     if (can.read(msg)) 
          {  
            printf("recu  :id: \t %i data: \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n", msg.id, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);             
            if (mail_box.full())  {   printf("error mail_box.full\n" ); }
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
                    
                    //  printf("envoye  :id: %i data: \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n",msg.data[0], ack.data[0], ack.data[1], ack.data[2], ack.data[3], ack.data[4], ack.data[5], ack.data[6], ack.data[7]);
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
                      /*cmde == 150 sert le cocktail 
                    pour les cocktails, on peut mettre 3 actions dans le message */ 
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
            
          }
    /*fonctions Raspberry*/
      char buf_rspby[256] = {0};    
       
      while ( rspby.readable()) 
       {  
          ThisThread::sleep_for(chrono::milliseconds(200)); /*il faut attendre que le buffer se remplisse*/
            rspby.read(buf_rspby, sizeof(buf_rspby)); } 
          if (buf_rspby[0]!=0){ 
            printf("rspby %s  \n", buf_rspby);
            printf("hex 0x%x  \n", buf_rspby[0]);
            //printf("id %i \t data : %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n", buf_rspby[6], buf_rspby[10], buf_rspby[11], buf_rspby[12], buf_rspby[13], buf_rspby[14], buf_rspby[15], buf_rspby[16], buf_rspby[17]);

          if (buf_rspby[0]==0xAA )
                {/*envoi le message CAN*/
                send_buffer( buf_rspby[1], buf_rspby[2], buf_rspby[3], buf_rspby[4], buf_rspby[5], buf_rspby[6], buf_rspby[7], buf_rspby[8], buf_rspby[9] ); 
                printf("id %i \t data : %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n", buf_rspby[1], buf_rspby[2], buf_rspby[3], buf_rspby[4], buf_rspby[5], buf_rspby[6], buf_rspby[7], buf_rspby[8], buf_rspby[9]);
                } 
          else if (buf_rspby[0]==0x99 )
                {
                  /*envoi le message CAN*/
                  //clearscreen();
                }
          else
                {
                  printf("text id %i \t data : %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n", buf_rspby[1], buf_rspby[2], buf_rspby[3], buf_rspby[4], buf_rspby[5], buf_rspby[6], buf_rspby[7], buf_rspby[8], buf_rspby[9]);
                  //writeToNextion(buf_rspby);
                }
       }

  }
}
