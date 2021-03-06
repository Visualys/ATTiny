#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_serial.c"
#include "lib_wait.c"


void main(void) {
    //eeprom_update_byte((uint8_t*) 1, 134 );                          // Set the OSCCAL memory address (line to remove when set)
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA0, "ATTiny84 started.\n", 115200);
    
    
    PORTA |=0b00000110;
    while(1){
		if(!(PINA & 0b00000010)){OSCCAL++;}
		if(!(PINA & 0b00000100)){OSCCAL--;}
		serial_send(PA0, "...\n", 115200);
        wait_ms(1000);	
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:template.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
