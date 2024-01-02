#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "./usartUtils.h"
#include "./ADC_Utils.h"



int main(void) {
    initUSART();

    DDRB |= (1<<DDB5);

    while(1){
      transmitMessage("Hello\n\r");

      PORTB |= (1<<DDB5);
      _delay_ms(500);

      PORTB &= ~(1<<DDB5);
      _delay_ms(500);
    }
}
