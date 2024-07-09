#include "ADC_Utils.h"
#include <avr/io.h>

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
