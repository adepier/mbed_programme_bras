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
    uint32_t i;
    uint32_t j;
    //initialise les buffers
    for (i = 0; i < BUFFER_CMDE_NB; i++)
            {
    for (j = 0; j < BUFFER_CMDE_SIZE; j++)
      {
           cmd_list[j][i] = '\0';
        }
        }
    string s = "***\n";
    char line[s.length() + 1];
    strcpy(line, s.c_str());
    serial_port.write(line, sizeof(line));
    int flag_read = 0;
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
                { // on met a vide le reste de la ligne
                    // for (j = nb_char_recu; j < BUFFER_CMDE_SIZE; j++)
                    // {
                    //     cmd_list[j][nb_cmde_entree] = '\0';
                    // }
                    // on passe a la ligne suivante
                    nb_char_recu = 0;
                    nb_cmde_entree = (nb_cmde_entree + 1) % BUFFER_CMDE_NB; // % = modulo
                }
                flag_read = -1;
            }

            // Echo the input back to the terminal.
            // serial_port.write(buf, num);
            // on attend un peu
            ThisThread::sleep_for(chrono::milliseconds(10));
        }

        if (flag_read)
        {
            
            //serial_port.write(line, sizeof(line));
            for (i = 0; i < BUFFER_CMDE_NB; i++)
            {
                for (j = 0; j < BUFFER_CMDE_SIZE; j++)
                {
                    buf_cmde[j] = cmd_list[j][nb_cmde_envoyee];
                    // on vide la ligne
                    cmd_list[j][nb_cmde_envoyee] = '\0';
                } 
                serial_port.write(line, sizeof(line)); 
                serial_port.write(buf_cmde, BUFFER_CMDE_SIZE);

                 
                nb_cmde_envoyee = (nb_cmde_envoyee + 1) % BUFFER_CMDE_NB; // % = modulo
            }
            
            flag_read = 0;
        }
    }
}
// #include "mbed.h"

// // Create a DigitalOutput object to toggle an LED whenever data is received.
// static DigitalOut led(LED1);

// // Create a BufferedSerial object with a default baud rate.
// static BufferedSerial serial_port(USBTX, USBRX, 9600);
// #define MAXIMUM_BUFFER_SIZE 32

// int main(void)
// { // Application buffer to receive the data
//     string s1 = "test demarrage\0";
//     char buffer[s1.length() + 1];
//     strcpy(buffer, s1.c_str());
//     printf("%s\n", buffer);
//     char buf[MAXIMUM_BUFFER_SIZE] = {0};

//     string s = "";
//     string cmd_list[5] = {0};
//     uint32_t nb_cmde_entree = 0;
//     uint32_t nb_cmde_envoyee = 0;
//     uint32_t i;

//     while (1)
//     {
//         uint32_t num = serial_port.read(&buf, sizeof(&buf));
//         if (num>0)
//         {
//             // Toggle the LED.
//             led = !led;
//             printf("num %i\n", num);
//             for (i = 0; i < num; i++)
//             {

//                 s = s + buf[i];

//                 if (s.find(";") )
//                 {
//                     s = s + "\n";
//                     cmd_list[nb_cmde_entree] = s;
//                     nb_cmde_entree = (nb_cmde_entree + 1) % 5; // % = modulo

//                     s = "";
//                 }
//             }
//             serial_port.write(s.c_str(), sizeof(s));
//             //
//         }

//         // on attend un peu
//         // ThisThread::sleep_for(chrono::milliseconds(1000));
//         if (nb_cmde_entree > 3)
//         {
//             for (i = nb_cmde_envoyee; i < nb_cmde_entree; i++)
//             {
//                 serial_port.write(cmd_list[i].c_str(), cmd_list[i].length());
//                 nb_cmde_envoyee = (nb_cmde_envoyee + 1) % 5; // % = modulo
//             }
//         }

//         // printf("s= %s / nb char %i\n",s.c_str(),s.length());
//         // char return_char[s.length()];
//         // strcpy(return_char, s.c_str());
//         // if (sizeof(return_char)  > 0)
//         // {
//         //     s = "\n" + s;
//         //      serial_port.write(s.c_str(), sizeof(s));

//         //       serial_port.write(return_char, sizeof(return_char));
//         //     s = "";
//         // }
//     }
// }