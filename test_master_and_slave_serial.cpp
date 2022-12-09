#include <mbed.h>
 
#define BUILD_SERIAL_SLAVE 1 // Build for slave =1 or master=0 of this example
 
#if BUILD_SERIAL_SLAVE
// Slave side of the example

// Create a DigitalOutput object to toggle an LED whenever data is received.
static DigitalOut led(LED1);

// Create a BufferedSerial object with a default baud rate.
//  static BufferedSerial serial_port(USBTX, USBRX,9600);
static BufferedSerial serial_port(D1, D0,9600);

int main(void)
{
   
    // Application buffer to receive the data
    char buf[1] = {0};

    while (1) {
        if (uint32_t num = serial_port.read(buf, sizeof(buf))) {
            // Toggle the LED.
            led = !led;

            // Echo the input back to the terminal.
            serial_port.write(buf, num);
        }
    }
}

#else
// Master side of the example
// Create a serial object
static BufferedSerial pc(USBTX, USBRX);
static BufferedSerial uart(D1, D0,9600);

int flag=0;

int main(void)
{   
    string s = "Echoes back to the screen anything you type\0";
    char buffer[s.length() + 1];
    strcpy(buffer, s.c_str()); 
    printf("%s\n", buffer);  
    while (1) {
        if (pc.readable()) {
            ThisThread::sleep_for(100);
            char buffer1[1];
            pc.read(&buffer1, 1);
            uart.write(&buffer1,1);
            flag=1;
            ThisThread::sleep_for(100);
            while (flag==1) {
            if (uart.readable()) {
                ThisThread::sleep_for(100);
                char buffer2[1];
                uart.read(&buffer2, 1);
                pc.write(&buffer2, 1);
                flag=0;
                }
            }            
        }
    }
}

#endif