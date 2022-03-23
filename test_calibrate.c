#include <avr/io.h>
#include "lib_wait.c"
#include "lib_serialcom.c"
#include <string.h>

int main(void) {
	char s[50];
	DDRB |= (1<<PB0)|(1<<PB4);     // PB0 as output (oscillo), PB4 is TX
	TCCR0A = (1<<COM0B0)|(1<<WGM01)|(1<<WGM00); // Fast PWM mode / Toggle COM.A
	OCR0A = 99;
	TCCR0B = (1<<WGM02) | 1;   // 1/1 clock 
	while(1) {
		PORTB |= (1 << PB1) | (1 << PB2);  // pull-up resistor on PB1 & PB2
		if(!(PINB & PB1)){
			wait_ms(10);
			while(!(PINB & PB1));
			OSCCAL++;
			sprintf(s,"OSCCAL = %i\r\n", OSCCAL);
			serialcom_send(PB4, s);
			}
		if(!(PINB & PB2)){
			wait_ms(10);
			while(!(PINB & PB2));
			OSCCAL--;
			sprintf(s,"OSCCAL = %i\r\n", OSCCAL);
			serialcom_send(PB4, s);
			}
		}
	}
