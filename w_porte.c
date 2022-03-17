#include <avr/io.h>
#include "lib_wait.c"

int main(void) {
    uint8_t var, count;
    DDRB = (1 << PB1) | (1 << PB2);     // PB1, PB2 as output
    PORTB = (1 << PB3) | (1 << PB4);    // PB3, PB4 set pull-up resistor
    while(1){
        while(PINB & 0b00011000 == 0);      // wait a push button
        if(PINB & (1 << PB3)) {
            var = (1 << PB1);
        }else{
            var = (1 << PB2);
            }
        wait_ms(50);                        // debounce time
        PORTB |= var;                       // start the motor
        for(count=0;count<3;count++){       // count number of turns
            while(!(PINB & (1 << PB0)));
            wait_ms(20);
            while(PINB & (1 << PB0));
            wait_ms(20);
            }
        PORTB &= ~var;                      // stop the motor
        }
    }
