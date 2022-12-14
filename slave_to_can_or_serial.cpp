/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

// Maximum number of element the application buffer can contain
#define MAXIMUM_BUFFER_SIZE 32
#define BUFFER_CMDE_SIZE 64
#define BUFFER_CMDE_NB 5

// Create a DigitalOutput object to toggle an LED whenever data is received.
static DigitalOut led(LED1);

// Create a BufferedSerial object with a default baud rate.
static BufferedSerial serial_port(USBTX, USBRX, 9600);
int reception_cmde(){
    
}
int main(void)
{
    serial_port.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1);
    // Application buffer to receive the data
    char buf[MAXIMUM_BUFFER_SIZE] = {0};
    char buf_cmde[BUFFER_CMDE_SIZE] = {0};
    char cmd_list[BUFFER_CMDE_SIZE][BUFFER_CMDE_NB] = {0};
    uint32_t nb_char_recu = 0;
    uint32_t nb_cmde_entree = 0;
    uint32_t nb_cmde_envoyee = 0;
    int nb_cmde = 0;
    uint32_t i;
    uint32_t j;
    
    string s = "\n";
    char line[s.length() + 1];
    strcpy(line, s.c_str());
    serial_port.write(line, sizeof(line)); 
    while (1)
    {
        while (serial_port.readable() == 1)
        {
            uint32_t num = serial_port.read(buf, MAXIMUM_BUFFER_SIZE);
            // Toggle the LED.
            led = !led;

            for (i = 0; i < num; i++)
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

        
            while (nb_cmde>0)  
            {
                for (j = 0; j < BUFFER_CMDE_SIZE; j++)
                {
                    buf_cmde[j] = cmd_list[j][nb_cmde_envoyee];
                    // on vide la ligne
                    cmd_list[j][nb_cmde_envoyee] = '\0';
                }  
                serial_port.write(line, sizeof(line)); //new line
                serial_port.write(buf_cmde , BUFFER_CMDE_SIZE); 
                nb_cmde_envoyee = (nb_cmde_envoyee + 1) % BUFFER_CMDE_NB; // % = modulo 
                nb_cmde--;
               
            }
             
        
    }
}