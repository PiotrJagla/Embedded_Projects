#include <TM1650.h>
#include <two_wire.h>


unsigned char digits[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};



void TM1650_write_digit(uint8_t segment, uint8_t digit) {
	begin_transmission(TM1650_ADDRESS + segment);
	write(digit);
	end_transmission();
}

void TM1650_init() {
	begin_transmission(TM1650_ADDRESS);
	end_transmission();
	for(int i = 0 ; i < 4 ; ++i) {
		begin_transmission(TM1650_CTRL_CMD + i);
		uint8_t ctrl = (0&TM1650_MSK_ONOFF) | TM1650_BIT_DOT;
		write(ctrl);
		end_transmission();
	}
}
