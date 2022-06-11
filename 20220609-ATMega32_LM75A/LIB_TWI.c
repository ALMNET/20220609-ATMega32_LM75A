
#include <avr/io.h>
#include "LIB_TWI.h"

/**
 * @brief   Initializes the I2C Interface
 * 
 * @params  None
 * 
 * @return  None
 */

void setup_I2C() {
    DDRC &= 0x0f;
    PORTC |= 0x03;  // Port C pins 4 and 5 set to input with pullups
    TWBR = 193;
    TWSR = 0;
}


/**
 * Wait for the current I2C operation to finish.
 * This possible allows the processor to wait forever, but if the I2C bus
 * is enabled it will eventually finish.
 */
void I2C_wait() {
    while ((TWCR & _BV(TWINT)) == 0) {
        ;
    }
}

/**
 * Send an I2C start bit.
 * 
 * @return true if the start bit was successfully transmitted
 */
int I2C_Start() {
    // Send I2C Start flag
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
    I2C_wait();
    return ((TWSR & 0xf8) == 0x08);
}

/**
 * Send an I2C address byte and R/W flag
 * 
 * @param addr I2C address of the slave
 * @param rw whether to read or write: 0 to write, 1 to read
 * @return true if the address byte was successfully transmitted
 */
int I2C_SLA(uint8_t addr, uint8_t rw) {
    // Send I2C slave address
    TWDR = (addr << 1) | (rw & 1);
    TWCR = _BV(TWINT) | _BV(TWEN);
    I2C_wait();
    return ((TWSR & 0xf8) == 0x18);
}

/**
 * Send a byte of data through the I2C bus
 * 
 * @param data data to transmit
 * @return true if the data was successfully transmitted
 */
int I2C_Send(uint8_t data) {
    // Send I2C data byte
    TWDR = data;
    TWCR = _BV(TWINT) | _BV(TWEN);
    I2C_wait();
    return ((TWSR & 0xf8) == 0x28);
}

/**
 * Send the stop flag on the I2C bus
 */
void I2C_Stop() {
    // Send I2C Stop flag
    TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
    for (volatile long x = 0; x < 100; x++) {
        ;
    }
}

/**
 * Check if there is a device on the I2C bus at address
 * 
 * @param addr address to check if a device exists there.
 * @return true if a device acknowledges the address probe
 */
int I2C_CheckAddress(uint8_t addr) {
    int ret;
    
    ret = I2C_Start() & I2C_SLA(addr, I2C_WRITE);
    if (ret) {
        I2C_Stop();
    }
    return ret;
}