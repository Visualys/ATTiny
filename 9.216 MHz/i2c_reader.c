#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_str.c"
#include "lib_i2c.c"

char s[50];
uint8_t v=0;
uint32_t l=0;

void main(void) {
    eeprom_update_byte((uint8_t*) 1, 140 );                          // Set the OSCCAL memory address (line to remove when set)
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA0, "ATTiny84 started.\n", 115200);
    
    while(1){
		v=i2c_init(PB0,PB1,0x76);
		longtostr(v,s);
		serial_send(PA0, s, 115200);
		serial_send(PA0, "\n", 115200);
		if(v){
			serial_send(PA0, "BME connected.\n", 115200);
		}else{
			serial_send(PA0, "BME not connected.\n", 115200);
			}
        wait_ms(1000);	
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:i2c_reader.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
