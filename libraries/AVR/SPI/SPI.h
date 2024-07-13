#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <stdint.h>

#define CS PB2
#define MOSI PB3
#define SCK PB5
#define RST PB0
#define DC PB1

#define DDR_SPI DDRB

void SPI_MasterInit(void); 
void SPI_MasterTransmit(uint8_t dat);

#endif
