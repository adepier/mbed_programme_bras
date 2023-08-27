//#include "config_motor.hpp"
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

  while (true)
  {
     if (can.read(msg)) {
            if(msg.id==DEVICE_ID)
            {printf("Message received:  from %i valeur: %i \n", msg.data[0], msg.data[1]);
            //si la commande est supérieur à 100, on renvoie les valeurs avec l'accusé reception
            // msg.data[1] = 100 -> donne la position bras/ renvoie angles = DATA[1..5] (parametres: destinataire DATA[2]  )
            // msg.data[1] = 101 -> donne la position main/ renvoie ouvert/fermé = DATA[1] (parametres: destinataire DATA[2]  )
              
               if (mail_box.full())
              //si il y a encore des commandes dans le mail, on renvoie 0 = le bras bouge
              { // Envoyer l'accusé de réception avec les valeurs
                 char data[8] = {(char)DEVICE_ID, (char)ACK_ID
                                               , (char) 0, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0/*valeur non valides le bras bouge*/
                                               };
                CANMessage ack(msg.data[0], data, 8);
                // printf("envoye  :id: %i data: \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n",msg.data[0], ack.data[0], ack.data[1], ack.data[2], ack.data[3], ack.data[4], ack.data[5], ack.data[6], ack.data[7]);
                if(can.write(ack)==0){can.reset(); printf("error write ack\n" );};
                }
              else if (msg.data[1] == 100)
              {//cmde = 100 -> donne la position bras/ renvoie les angles = data_3..8  (parametres: destinataire data_1  )
                // Envoyer l'accusé de réception avec les valeurs
                char data[8] = {(char)DEVICE_ID, (char)ACK_ID
                                                , (char) init_position_done  /*l'init a été fait*/
                                                , (char)motor_epaule_a_plat.get_angle()
                                                , (char)motor_epaule_haut.get_angle()
                                                , (char)motor_coude.get_angle()
                                                , (char)motor_poignet.get_angle()
                                                , (char)motor_poignet_haut.get_angle() 
                                                };
                CANMessage ack(msg.data[0], data, 8);
                 if(can.write(ack)==0){can.reset(); printf("error write current_position\n" );};
                 printf("envoye  :id: %i data: \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n",msg.data[0], ack.data[0], ack.data[1], ack.data[2], ack.data[3], ack.data[4], ack.data[5], ack.data[6], ack.data[7]);

              }
              else if (msg.data[1] == 101)
              {
                // Envoyer l'accusé de réception avec la position des doigts sur 6 bits
                int pos = 0;
                pos = doigt_0.get_position() |
                      doigt_1.get_position()<<1 |
                      doigt_2.get_position()<<2 |
                      doigt_3.get_position()<<3 |
                      doigt_4.get_position()<<4 |
                      doigt_5.get_position()<<5 ;
                char data[4] = {(char)DEVICE_ID, (char)ACK_ID
                                                , '1' /*angles valides le bras est a l'arret*/
                                                , (char)pos};
                CANMessage ack(msg.data[0], data, 4);
                can.write(ack);
                //pour le decoder 
                //  int i;
                // for (i = 0; i < 8; i++) {
                //     printf("%d", !!((result << i) & 0x80));
                // }
                // printf("\n");
              }
              else  if (msg.data[1] == 200) 
                          { 
                             // Envoyer l'accusé de réception
                             char  data[8] = {(char) DEVICE_ID, (char)ACK_ID, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0};
                            // CANMessage ack(msg.data[0], data ,8); 
                            CANMessage ack(msg.data[0],  CANStandard); 
                            // printf("envoye  :id: %i data: \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n",msg.data[0], ack.data[0], ack.data[1], ack.data[2], ack.data[3], ack.data[4], ack.data[5], ack.data[6], ack.data[7]);
                            if(can.write(ack)==0){ can.reset(); printf("error write ack\n" );} 
                            // printf("accuse reception run envoye \n" );
                            //fin de la trame, on lance la fonction
                           else { 
                            // printf("run_carrousel\n" );
                            run_bras();
                            // printf("fin run_carrousel\n" );
                            }
                          }
            
           
            else {
                // Envoyer l'accusé de réception
                        char  data[8] = {(char) DEVICE_ID, (char)ACK_ID, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0};
                        CANMessage ack(msg.data[0], data ,8); 
                        // printf("envoye  :id: %i data: \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n",msg.data[0], ack.data[0], ack.data[1], ack.data[2], ack.data[3], ack.data[4], ack.data[5], ack.data[6], ack.data[7]);
                        if(can.write(ack)==0){ printf("error write ack\n" );} 
                          // printf("accuse reception envoye \n" );
                        else 
                        {
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
}
}
