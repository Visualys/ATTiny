/*
           +-----+
      RST -|o    |- VCC *
      PB3 -|     |- PB2
      PB4 -| T45 |- PB1
    * GND -|     |- PB0 *
           +-----+
*/

#define F_CPU 1000000UL
#include <avr/io.h>
#include "lib_wait.c"
#include "lib_standby.c"

int main(void) {
	while(1) {
	    DDRB = 0b00011111;
		PORTB = 0b00011111;
		wait_s(10);
	    DDRB = 0b00000000;
		PORTB = 0b00000000;
		standby_min(5);
		}
	}
