#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_serial.c"

void main(void) {
    eeprom_update_byte((uint8_t*) 1, 110 );                          // Set the OSCCAL memory address (line to remove when set)
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    serial_send(PA0, "ATTiny84 started.\n", 115200);
    }



/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:template.hex:i -U lfuse:w:0xE2:m -U hfuse:w:0xD7:m
*/
