#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_serial.c"
#include "lib_wait.c"

/*
   PA0 : SERIAL TX
   PA1 : ADD ONE TO OSCCAL
   PA2 : REMOVE ONE TO OSCCAL
   PA3 : RECORD TO EEPROM
*/

char s[50];

void main(void) {
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA0, "ATTiny84 started.\n", 115200);
    PORTA |=0b00001110;
    while(1){
        if(!(PINA & 0b00000010)){OSCCAL++;}
        if(!(PINA & 0b00000100)){OSCCAL--;}
        if(!(PINA & 0b00001000)){eeprom_update_byte((uint8_t*) 1, OSCCAL );}
        sprintf(s, "OSCCAL = %i\n", OSCCAL);
        serial_send(PA0, s, 115200);
        wait_ms(250);	
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:template.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
