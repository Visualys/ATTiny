#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_BME.c"

uint8_t v;
char s[50];

void main(void) {
    //eeprom_update_byte((uint8_t*) 1, 140 );                        // Set the OSCCAL memory address (line to remove when set)
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA0, "ATTiny84 started.\n", 115200);
    while(1){
        v=BME_init(PB0,PB1);
        if(v){
            serial_send(PA0, "BME connected.\n", 115200);
            BME_measure();
          
            sprintf(s, "Temperature : %i\n", BME_temperature());              
            serial_send(PA0, s, 115200);
            
            sprintf(s, "Pressure : %i\n", BME_pressure());              
            serial_send(PA0, s, 115200);

            sprintf(s, "Humidite : %i\n", BME_humidity());              
            serial_send(PA0, s, 115200);
        }else{
            serial_send(PA0, "BME not connected.\n", 115200);
            }
        wait_ms(1000);	
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:i2c_reader.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
