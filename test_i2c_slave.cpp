
/** An I2C Slave, used for communicating with an I2C Master device.

@note Synchronization level: Not protected

Example Simple I2C slave and master (requires two Mbed-boards):
*/
#include <mbed.h> 
#define SLAVE_ADDR 0xA0
#define BUFFER_SIZE 6


// Slave side of the example
// Create a serial object
// static BufferedSerial pc(USBTX, USBRX,9600);
    
   
 
I2CSlave slave(I2C_SDA, I2C_SCL);
int main() { 
    printf(" printf: I am the slave!");
//   char msg[] = "I am the slave!\n"; 
//    pc.write(msg, sizeof(msg));
    char buf[BUFFER_SIZE] = "ABCDE";
    slave.address(SLAVE_ADDR);
    while (1) {
        int i = slave.receive();
        switch (i) {
            case I2CSlave::ReadAddressed:
                // Write back the buffer from the master
                slave.write(buf, BUFFER_SIZE);
                printf("Written to master (addressed): %s\n", buf);
                break;

            case I2CSlave::WriteGeneral:
                slave.read(buf, BUFFER_SIZE);
                printf("Read from master (general): %s\n", buf);
                break;

            case I2CSlave::WriteAddressed:
                slave.read(buf, BUFFER_SIZE);
                printf("Read from master (addressed): %s\n", buf);
                break;
        }
    }
}


