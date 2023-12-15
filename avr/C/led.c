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


int main(void) {
    initUSART();
    DDRB = DDRB | (1<< DDB5);
    DDRD = DDRD & ~(1<< DDD2);
    PORTD = PORTD | (1<<DDD2);

    while(1){
      
      int sw = (PIND & (1<<PIND2))?1:0;
      //int sw = PIND & (1<<PIND2);
      int Xread= ADCread(0);
      int Yread= ADCread(1);

      char SWbuffer[16] = {};
      char Xbuffer[16] = {};
      char Ybuffer[16] = {};

      sprintf(SWbuffer, "switch: %d\n", sw);
      sprintf(Xbuffer, "X-axis: %d\n", Xread);
      sprintf(Ybuffer, "Y-axis: %d\n", Yread);

      transmitMessage(SWbuffer);
      transmitMessage(Xbuffer);
      transmitMessage(Ybuffer);

      _delay_ms(50);

    }
}
