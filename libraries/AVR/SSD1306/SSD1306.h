#ifndef SSD1306_H
#define SSD1306_H

#include <avr/io.h>
#include <stdint.h>

#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON  0xAF
#define SSD1306_SETDISPLAYALLON 0xA5


#define RST PB0
#define DC PB1

void SSD1306_send_command(uint8_t data); 
void SSD1306_init_display();

#endif
