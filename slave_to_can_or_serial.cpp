#include "mbed.h"

// Maximum number of element the application buffer can contain
#define MAXIMUM_BUFFER_SIZE 32
#define BUFFER_CMDE_SIZE 64
#define BUFFER_CMDE_NB 5
#define BUFFER_I2C 5
#define I2C_NOSTRETCH_ENABLE I2C_CR1_NOSTRETCH
// Application buffer to receive the data
char buf[MAXIMUM_BUFFER_SIZE] = {0};
char cmd_list[BUFFER_CMDE_SIZE][BUFFER_CMDE_NB] = {0};
char buf_cmde[BUFFER_CMDE_SIZE] = {0};
char bufi2c[BUFFER_I2C] = {0};
int nb_char_recu = 0;
int nb_cmde_entree = 0;
int nb_cmde_envoyee = 0;
int nb_cmde = 0;

// Create a BufferedSerial object with a default baud rate.
static BufferedSerial serial_port(USBTX, USBRX, 9600);

#define SLAVE_ADDR 0xA0
#define I2C_SDA PB_7
#define I2C_SCL PB_6
I2CSlave slave(I2C_SDA, I2C_SCL);

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
        // debug
        // serial_port.write(buf, num);
        //  on attend un peu
        ThisThread::sleep_for(chrono::milliseconds(10));
    }
    printf("nb cmde: %i\n", nb_cmde_entree - nb_cmde_envoyee);
}

void envoi_cmde()
{

    if (nb_cmde > 0)
    {
        for (int i = 0; i < BUFFER_CMDE_SIZE; i++)
        {
            buf_cmde[i] = cmd_list[i][nb_cmde_envoyee];
            // on vide la ligne
            cmd_list[i][nb_cmde_envoyee] = '\0';
        }

        serial_port.write(buf_cmde, BUFFER_CMDE_SIZE);
        // Write back the buffer from the master
        slave.write(buf_cmde, BUFFER_CMDE_SIZE);
        printf("Written to master (addressed): %s\n", buf_cmde);
        nb_cmde_envoyee = (nb_cmde_envoyee + 1) % BUFFER_CMDE_NB; // % = modulo
        nb_cmde--;
    }
}
int main(void)
{
    slave.address(SLAVE_ADDR);
    while (1)
    {
        reception_cmde();
        ThisThread::sleep_for(chrono::milliseconds(1000));
        int i = slave.receive();
        switch (i)
        {
        case I2CSlave::ReadAddressed:
            // Write back the buffer from the master
            envoi_cmde(); // slave.write(buf, BUFFER_SIZE);
            break;
        case I2CSlave::WriteGeneral:
            slave.read(bufi2c, BUFFER_I2C);
            printf("Read from master (general): %s\n", bufi2c);
            break;
        case I2CSlave::WriteAddressed:
            slave.read(bufi2c, BUFFER_I2C);
            printf("Read from master (addressed): %s\n", bufi2c);
            break;
        }

        ;
    }
}