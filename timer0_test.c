#define F_CPU 1000000UL
#include <avr/io.h>


int main(void) {
	DDRB =(1<<PB1)|(1<<PB0); // set PB1 and PB0 as output to enable PWM generation
	OCR0A = 128;
	OCR0B = 32;
	TCCR0A=0x00;  
	TCCR0A |= (1<<WGM01)|(1<<WGM00)|(1<<COM0B1)|(0<<COM0B0)|(0<<COM0A1)|(0<<COM0A0);
	TCNT0 = 0; //set counter to zero
	TCCR0B = (1<<WGM02) | 3; //start timer0 with 1/64 speed
	while(1){
		if(TIFR & (1<<TOV0)) {
			TIFR |= (1<<TOV0); //reset overflow flag
			if(OCR0B==96) {
				OCR0B = 32;
				}else{
				OCR0B = 96;
				}
			}
		}
	}


// avrdude -p ATtiny45 -c stk500 -P /dev/ttyACM0 -U flash:w:timer0_test.hex:i -U lfuse:w:0xE2:m
