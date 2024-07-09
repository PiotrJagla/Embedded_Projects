#include <two_wire.h>

uint8_t tw_write_length = 0;
uint8_t tw_address = 0;
bool read = false;
uint8_t tw_buffer[BUFFER_LENGTH] = {};


void begin_transmission(uint8_t address) {
	tw_write_length = 0;
	tw_address = address << 1;
}

void write(uint8_t data) {
	tw_buffer[tw_write_length++] = data;
}

void I2C_start_condition() {
	TWCR |= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
}
void I2C_stop_condition() {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}
void I2C_wait_for_twint_flag() {
	while(!(TWCR & 1<<TWINT));
}

I2C_STATUS end_transmission() {
	I2C_start_condition();


	I2C_wait_for_twint_flag();
	
	if((TWSR&0xF8) != I2C_START){
		return I2C_NOT_STARTED;
	}
	TWDR = tw_address;
	TWCR = (1<<TWINT)|(1<<TWEN);

	I2C_wait_for_twint_flag();

	if((TWSR&0xF8) != I2C_SL_SEND_ACK_RECV){ // See if ACK is received
		return ADDRESS_ACK_NOT_RECEIVED;
	}

	for(int i = 0 ; i < tw_write_length ; i++) {
		TWDR = tw_buffer[i];
		TWCR = (1<<TWINT)|(1<<TWEN);

		while(!(TWCR & (1<<TWINT))); // Wait for data transmittion to end

		//if((TWSR&0xF8) != I2C_DATA_SEND_ACK_RECV){ // See if ACK is received
		if((TWSR&0xF8) != I2C_DATA_SEND_ACK_RECV){ // See if ACK is received
			return DATA_ACK_NOT_RECEIVED;
		}
	}

	I2C_stop_condition();
	return SUCCESS;
}
 
void I2C_setup() {
	//Enable pull up resistors on SDA and SCL
	PORTB |= (1<<PC5);
	PORTB |= (1<<PC4);

	TWCR |= (1<<TWEN) | (1<<TWEA) | (1<<TWIE); // Enable I2C
	
	TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;
}
 
void I2C_stop() {
	TWCR &= ~((1<<TWEN) | (1<<TWEA) | (1<<TWIE)); // disable I2C
	
	//Deactivate pullups
	PORTB &= ~(1<<PC5);
	PORTB &= ~(1<<PC4);
}
