#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 16000000UL
#define BAUD_RATE 115200UL
#define BAUD ((F_CPU)/(BAUD_RATE*8UL)-1)
#define CHAR_OFFSET 48


void initUSART(void) {  
  UBRR0H = (BAUD>>8);  
  UBRR0L = BAUD;       

  UCSR0A |= (1 << U2X0);
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_transmit(unsigned char data) {

  while(!(UCSR0A & (1<<UDRE0))){}

  UDR0 = data;
}

void transmitMessage(char* message) {
  for(int i = 0 ; message[i] != '\0' ; ++i){
    USART_transmit(message[i]);
  }
}

int ADCread(int adctouse) {
  int ADCval;

  ADMUX = adctouse;
  ADMUX |= (1<<REFS0);
  ADMUX &= ~(1<<ADLAR);

  ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

  ADCSRA |= (1<<ADEN); 
  ADCSRA |= (1<<ADSC);

  while(ADCSRA & (1<<ADSC)) {}

  ADCval = ADCL;
  ADCval = (ADCH << 8) + ADCval;
  return ADCval;
}

char* intToChar(int val) { 
  char buff[6] = {};
  int i = 0;
  int divisor = 1;
  while(val != 0) {
    buff[i++] = (char)((val%10)+CHAR_OFFSET);
    divisor *= 10;
    val /= divisor;
  }
  buff[i] = '\n';
  return buff;
}

int main(void) {
    initUSART();
    DDRB = DDRB | (1<< DDB5);

    while(1){
      
      int val = ADCread(5);
      char buffer[10] = {};
      sprintf(buffer, "%d\n", val);
      transmitMessage(buffer);


      PORTB = PORTB | (1<<PORTB5);

      _delay_ms(200);

      PORTB = PORTB & (~(1 << PORTB5));
      _delay_ms(2000);

    }
}
