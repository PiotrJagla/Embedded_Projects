#ifndef TM1650_H
#define TM1650_H


#define TM1650_ADDRESS 0x34 
#define TM1650_CTRL_CMD 0x24
#define TM1650_DISP_CMD 0x68

#define TM1650_DISPLAY_BASE 0x34
#define TM1650_DCTRL_BASE   0x24
#define TM1650_NUM_DIGITS   16
#define TM1650_MAX_STRING   128

#define TM1650_BIT_ONOFF    0b00000001
#define TM1650_MSK_ONOFF    0b11111110
#define TM1650_BIT_DOT      0b00000001
#define TM1650_MSK_DOT      0b11110111
#define TM1650_DOT          0b10000000
#define TM1650_BRIGHT_SHIFT 4
#define TM1650_MSK_BRIGHT   0b10001111
#define TM1650_MIN_BRIGHT   0
#define TM1650_MAX_BRIGHT   7

void TM1650_write_digit(uint8_t segment, uint8_t digit); 
void TM1650_init();


#endif
