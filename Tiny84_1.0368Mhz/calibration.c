#include <avr/io.h>
#include "lib_eeprom.c"
#include "lib_serial.c"
#include "lib/lib_wait.c"
#include "lib_str.c"


// This Program is used to Synchronize CPU to 1.0368 MHz (956ns period)
// low fuse:22 to output clock on PB2
// PA5 --> Serial out (9600 bauds)

char s[40];
uint32_t v=0;

void main(void) {
	OSCCAL = eeprom_read(1);                                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA5, "ATTiny84 started.\n", 9600);
    
    PORTA |=0b00000111;
    while(1){
		if(!(PINA & 0b00000001)){eeprom_write(1, OSCCAL);}          // PA0 Save OSCCAL to EEPROM
		if(!(PINA & 0b00000010)){OSCCAL++;}
		if(!(PINA & 0b00000100)){OSCCAL--;}
		v=OSCCAL;
		longtostr(v, s);
		serial_send(PA5, "OSCCAL = ", 9600);
		serial_send(PA5, s, 9600);
		serial_send(PA5, "\n", 9600);
        wait_ms(500);	
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:template.hex:i -U lfuse:w:0x22:m -U hfuse:w:0xD7:m
*/




