
// #include "mbed.h"
#include "config_fonctions.hpp"

void onReceive();
void CANreceive();
int i = 0;
int a = 0;

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
    printf("envoi :id: \t %i data: \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n", msg_to_send.id, msg_to_send.data[0], msg_to_send.data[1], msg_to_send.data[2], msg_to_send.data[3], msg_to_send.data[4], msg_to_send.data[5], msg_to_send.data[6], msg_to_send.data[7]);
    if(can.write(msg_to_send)==0){  printf("error write\n" );  };
    // CANMessage msg_recu;
    // while ( msg_recu.id!=DEVICE_ID ){
    //     can.read(msg_recu);
    //     if(msg_recu.id==DEVICE_ID) {printf("recu  :id: \t %i data: \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \n", msg_recu.id, msg_recu.data[0], msg_recu.data[1], msg_recu.data[2], msg_recu.data[3], msg_recu.data[4], msg_recu.data[5], msg_recu.data[6], msg_recu.data[7]);}
    // } 
    // return msg_recu;
     
}

int main(void)
{ 
    printf("init\n");
    //can.attach(callback(&onReceive), CAN::RxIrq);

 

    int nb = i;
    while (1)
    {
        
         CANreceive();
            while (!mail_box.empty())
            {
                mail_t *mail = mail_box.try_get();

                if (mail != nullptr)
                {
                    printf("mail %i, %i, %i, %i, %i, %i \n", mail->cmde, mail->data_1, mail->data_2, mail->data_3, mail->data_4, mail->data_5);
                }

                mail_box.free(mail);
                
                // ThisThread::sleep_for(chrono::milliseconds(1s));
            }
           nb=i; 
           
         
    }

    return 0;
}

void onReceive()
{
printf("data:");
  //  i = i + 1;
    // CANreceive();
}

void CANreceive()
{
    CANMessage msg;

    if (can.read(msg))
    {
        // printf("data: %s,id:%d\n", msg.data, msg.id);
        // stocke les parametres dans un mail
        mail_t *mail = mail_box.try_alloc();
        mail->cmde = msg.data[1];
        mail->data_1 = msg.data[2];
        mail->data_2 = msg.data[3];
        mail->data_3 = msg.data[4];
        mail->data_4 = msg.data[5];
        mail->data_5 = msg.data[6];
        mail->data_6 = msg.data[7];

        mail_box.put(mail);
        send_buffer( 10, DEVICE_ID ,  200,  0 ,   0,  0,  0,   0,   0 ); /*ack*/
    }
}