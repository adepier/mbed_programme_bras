#include <mbed.h>
#include <mbed_wait_api.h>
#include <string.h>
#define BUILD_I2C_SLAVE 0 // Build for slave =1 or master=0 of this example
#define SLAVE_ADDR 0xA0
#define BUFFER_SIZE 6
#define I2C_SDA PA_10
#define I2C_SCL PA_9
#define 	I2C_NOSTRETCH_ENABLE   I2C_CR1_NOSTRETCH
/*****************use 4k7 pull-ups connected to SDA and SCL and to 3.3V but 5V is also working.!!!!!!!*/
#if BUILD_I2C_SLAVE
//********************************************* Slave side of the example
#if !DEVICE_I2CSLAVE
#error [NOT_SUPPORTED] I2C Slave is not supported
#endif
I2CSlave slave(I2C_SDA, I2C_SCL);
// Create a serial object
static BufferedSerial pc(USBTX, USBRX);
int main() { 
    char buf[BUFFER_SIZE] = "ABCDE";
    printf("I am the slave %s\n", buf ); 
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
#else
//******************************************* Master side of the example
I2C master(I2C_SDA, I2C_SCL);
static const char* to_send[] = { "abcde", "12345", "EFGHI" };
int main() {
    char buf[BUFFER_SIZE];
    int send_index = 0;
    while (1) {
        strcpy(buf, to_send[send_index]);
        // Write the new message to the slave
        if (master.write(SLAVE_ADDR, buf, BUFFER_SIZE)) {
            printf("Failed to write to slave!\n");
        } else {
            printf("Written to slave: %s\n", buf);
        }
        // Read what the slave has (should be identical)
        if (master.read(SLAVE_ADDR, buf, BUFFER_SIZE)) {
            printf("Failed to read from slave!\n");
        } else {
            printf("Read from slave: %s\n", buf);
        }
        // Change the message we're writing to the slave
        send_index++;
        if (send_index > 2) {
            send_index = 0;
        }
        wait_us(500000); // Wait 0.5s
    }
}
#endif