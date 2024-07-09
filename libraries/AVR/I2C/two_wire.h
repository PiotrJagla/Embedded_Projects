#ifndef TWO_WIRE_H
#define TWO_WIRE_H

#include <avr/io.h>
#include <stdbool.h>

typedef enum {
    SUCCESS,
    ADDRESS_ACK_NOT_RECEIVED,
    DATA_ACK_NOT_RECEIVED,
    I2C_NOT_STARTED,
    LAST_STATUS
}I2C_STATUS;

#define BUFFER_LENGTH 32
#define TWI_FREQ 100000L
#define I2C_START 0x08
#define I2C_REPEATED_START 0x10
#define I2C_SL_SEND_ACK_RECV 0x18
#define I2C_SL_SEND_ACK_NOT_RECV 0x20
#define I2C_DATA_SEND_ACK_RECV 0x28
#define I2C_DATA_SEND_ACK_NOT_RECT 0x30

extern uint8_t tw_buffer[];
extern uint8_t tw_write_length;
extern uint8_t tw_address;
extern bool read;


void I2C_setup();
void I2C_stop();
void begin_transmission(uint8_t address);
void write(uint8_t data);
I2C_STATUS end_transmission();
//uint8_t read(uint8_t address, uint8_t reg);




#endif
