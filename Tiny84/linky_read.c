#include <avr/io.h>
#include <stdlib.h>
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_convertions.c"

char s[50];
long prevHP, HP;

int serial_read_linky(uint8_t RX, char* msg) {
	uint8_t b = 0, n = 0, msgID = 0, pinRX = (1 << RX);
	DDRA &= ~pinRX;								// set pin as input
	TCCR0B = 0;									// stop timer
	TCCR0A = (1<<WGM01)|(1<<WGM00);				// set FastPWM mode
	TCNT0 = 0;									// timer val=0
	OCR0A = 59;
  while ( b != 10 ) {  
	while(!(PINA & pinRX) );
	TCCR0B = 0;									// stop timer
	TIFR0 |= (1<<TOV0);							// reset overflow flag
	TCNT0 = OCR0A >> 1;							// position trigger to the middle
	while(PINA & pinRX);						// wait start bit (HIGH to LOW)
	TCCR0B = (1<<WGM02) | 3;					// start timer with 1/8 speed
	while(!(TIFR0 & (1<<TOV0)));				// wait counter 0 overflow	
	TIFR0 |= (1<<TOV0);							// reset overflow flag
	b = 0;
    for(n=0; n<8; n++) {
		while(!(TIFR0 & (1<<TOV0)));			// wait counter 0 overflow			
		b >>= 1;
		if(PINA & pinRX) b += 128;				// store bit value
		TIFR0 |= (1<<TOV0);						// reset overflow flag
		}
    msg[msgID] = b & 127;
    msgID++;
    }
	TCCR0B = 0;									// stop timer
	TCNT0 = 0;									// timer val=0
	TIFR0 |= (1<<TOV0);							// reset overflow flag
  msg[msgID] = 0;
  } 


int main(void) {
	uint8_t counter=0;
	OSCCAL = 2;
	serial_send(PA1, "Starting.\n", 9600);
	
	while(1){
		serial_read_linky(PA0, s);
		if(startwith(s,"HCHP ")) { 
			HP = strtoint(s);
			}
		if(HP-prevHP>=5){
				s[0]=0;
				ltoa(HP, s, 10);
				stradd(s,"\n");
				serial_send(PA1, s, 9600);
				prevHP=HP;
			}
		}
	}
	
	
/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:linky_read.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
 
