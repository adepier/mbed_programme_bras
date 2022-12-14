#include "mbed.h"

// Maximum number of element the application buffer can contain
#define MAXIMUM_BUFFER_SIZE 32
#define BUFFER_CMDE_SIZE 64
#define BUFFER_CMDE_NB 5
// Application buffer to receive the data
char buf[MAXIMUM_BUFFER_SIZE] = {0};
char cmd_list[BUFFER_CMDE_SIZE][BUFFER_CMDE_NB] = {0};
char buf_cmde[BUFFER_CMDE_SIZE] = {0};
int nb_char_recu = 0;
int nb_cmde_entree = 0;
int nb_cmde_envoyee = 0;
int nb_cmde = 0;


// Create a BufferedSerial object with a default baud rate.
static BufferedSerial serial_port(USBTX, USBRX, 9600);



void reception_cmde()
{ 
    while (serial_port.readable() == 1)
    {
        int num = serial_port.read(buf, MAXIMUM_BUFFER_SIZE);
        
        for (int i = 0; i < num; i++)
        {
            cmd_list[nb_char_recu][nb_cmde_entree] = buf[i];
            nb_char_recu++;
            if (buf[i] == ';')
            {
                // on passe a la ligne suivante
                nb_char_recu = 0;
                nb_cmde_entree = (nb_cmde_entree + 1) % BUFFER_CMDE_NB; // % = modulo
                nb_cmde++;
            }
        }
        // on attend un peu
        ThisThread::sleep_for(chrono::milliseconds(10));
    }

}


void envoi_cmde()
{
    
    while (nb_cmde > 0)
    {
        for (int i = 0; i < BUFFER_CMDE_SIZE; i++)
        {
            buf_cmde[i] = cmd_list[i][nb_cmde_envoyee];
            // on vide la ligne
            cmd_list[i][nb_cmde_envoyee] = '\0';
        }
        buf_cmde[BUFFER_CMDE_SIZE-1]='\n';
        serial_port.write(buf_cmde, BUFFER_CMDE_SIZE);
        nb_cmde_envoyee = (nb_cmde_envoyee + 1) % BUFFER_CMDE_NB; // % = modulo
        nb_cmde--;
    }
}
int main(void)
{
    while (1)
    {
        reception_cmde();
        envoi_cmde();
    }
}