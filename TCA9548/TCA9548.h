// TCA9548 I2C Mux
#ifndef MBED_TCA9548_H
#define MBED_TCA9548_H

#include "mbed.h"

//I2C Address

#define DEV_ADDR (0x70<<1)

// CHANNEL VALUES
#define CH0 0x01
#define CH1 0x02
#define CH2 0x04
#define CH3 0x08
#define CH4 0x10
#define CH5 0x20
#define CH6 0x40
#define CH7 0x80


/** TCA9548 control class.
 *
 * Example:
 * @code
 * //Perform readings on two i2c devices with the same address
 * #include "TCA9548.h"
 * #include "mbed.h"
 * #include "VL6180.h"
 *
 * TCA9548 i2cMux(SDA, SCL);
 * VL6180 laser1(SDA, SCL); // on SD0 and SC0
 * VL6180 laser2(SDA, SCL); // on SD1 and SC1
 * Serial pc(USBTX, USBRX); //USB serial
 *
 * int main() {
 *   float reading;
 *   i2cMux.ch(CH0);
 *   reading = laser1.read();
 *   pc.printf("Laser 1: %f\n", reading);
 *   i2cMux.ch(CH1); //for two channels: i2cMux.ch(CH1 | CH5);
 *   pc.printf("Laser 2: %f\n", reading);
 *   reading = laser2.read();
 *       
 *   while(1) {
 *   }
 * }
 * @endcode
 */

class TCA9548 {
    private:
        I2C i2c;
        uint8_t channel;
        void i2cWrite8(uint8_t addr, char data);
    public:
        /** Initialize object with default i2c pins */
        TCA9548();
        
        /** Initialize object with specific i2c pins
        * 
        * @param i2c_sda SDA pin
        * @param i2c_scl SCL pin
        */
        TCA9548(PinName i2c_sda, PinName i2c_scl);
        
        /** Boot up the mux and change to initial channel (defaults to 0)
        * 
        * @param iCh Initial Channel
        * @return 
        *   1 if failed
        *`  0 if successful
        */
        bool init(uint8_t iCh);
        bool init();
        
        /** Select the i2c channel
        * 
        * @param newCh New channel for mux
        */
        void select_I2C(uint8_t newCh);
};

#endif