#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL
//#define F_CPU 1000000UL
#define BAUD_RATE 115200UL
#define BAUD ((F_CPU)/(BAUD_RATE*8UL)-1)


void initUSART(void) {                                
    UBRR0H = (BAUD>>8);  
    UBRR0L = BAUD;       // set baud rate

    UCSR0A |= (1 << U2X0);   // double transmission speed

                              /* Enable USART transmitter/receiver */
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);   /* 8 data bits, 1 stop bit */
}

void USART_transmit(unsigned char data) {

  //Wait for empty transmit buffer
  while(!(UCSR0A & (1<<UDRE0))){}

  //Put data into buffer, sends the data
  UDR0 = data;
}

void transmitMessage(char* message) {
  for(int i = 0 ; message[i] != '\0' ; ++i){
    USART_transmit(message[i]);
  }
}

int main(void) {
    initUSART();
    DDRB = DDRB | (1<< DDB5);

    while(1){
      

      transmitMessage("hello world\n");
      PORTB = PORTB | (1<<PORTB5);

      _delay_ms(1000);

      PORTB = PORTB & (~(1 << PORTB5));
      _delay_ms(1000);

    }
}
