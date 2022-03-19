#include <avr/io.h>
#include "lib_wait.c"


int motor(uint8_t var){
	uint8_t count;
	PORTB |= 0b00000001;                           // PB0 set pull-up
	PORTB |= var;
	for(count=0;count<12;count++){
		while(!(PINB & 0b00000001)); 
		wait_ms(50);
		while((PINB & 0b00000001)); 
		wait_ms(50);
		}
	PORTB &= ~var;
	PORTB &= 0b00000001;                           // stop PB0 pull-up
	}

int main(void) {
    DDRB = 0b00000110;                             // PB1, PB2 as output
    PORTB = 0b00011000;                            // PB3, PB4 set pull-up resistor
    while(1){
		while((PINB & 0b00011000) == 0b00011000);  // PB3 & PB4 still released
        if((PINB & (1 << PB3)) == 0){              // is PB3 pushed ?
			while((PINB & (1 << PB3)) == 0);
			wait_ms(20); 
			motor(1 << PB1);
			}
        if((PINB & (1 << PB4)) == 0){              // is PB4 pushed ?
			while((PINB & (1 << PB4)) == 0);
			wait_ms(20); 
			motor(1 << PB2);
			}
		}
    }

/*
avrdude -p ATtiny45 -c stk500 -P /dev/ttyACM0 -U flash:w:w_porte.hex:i -U lfuse:w:0xE2:m
*/
