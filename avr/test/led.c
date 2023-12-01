#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    //set DDB5 as output
    DDRB = DDRB | (1<< DDB5);

    while(1){
        //set PORTB to HIGH
        PORTB = PORTB | (1<<PORTB5);

        _delay_ms(2000);

        PORTB = PORTB & ~(1 << PORTB5);

        _delay_ms(4000);

    }
}
