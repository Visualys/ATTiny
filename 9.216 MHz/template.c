#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_str.c"

// This Program is used to Synchronize CPU to 9.216 MHz

char s[40];
uint32_t v=0;

void main(void) {
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA0, "ATTiny84 started.\n", 115200);
    
    
    PORTA |=0b00000111;
    while(1){
		if(!(PINA & 0b00000001)){eeprom_update_byte((uint8_t*) 1, OSCCAL);} // PA0 Save OSCCAL to EEPROM
		if(!(PINA & 0b00000010)){OSCCAL++;}
		if(!(PINA & 0b00000100)){OSCCAL--;}
		v=OSCCAL;
		longtostr(v, s);
		serial_send(PA5, "OSCCAL = ", 115200);
		serial_send(PA5, s, 115200);
		serial_send(PA5, "\n", 115200);
        wait_ms(1000);	
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:template.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/




