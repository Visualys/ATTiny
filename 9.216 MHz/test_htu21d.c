#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_i2c.c"

#define TX PA0
#define BAUDS 115200

char s[50];
uint8_t v;
uint16_t adc_t;
uint32_t t;

void main(void) {
    //eeprom_update_byte((uint8_t*) 1, 140 );                        // Set the OSCCAL memory address (line to remove when set)
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(TX, "ATTiny84 started.\n", BAUDS);
    while(1){
        v = i2c_init(PB0, PB1, 0x40);
        if(v){
            serial_send(TX, "HTU21D connected.\n", BAUDS);
            i2c_start();
            v = i2c_write(i2c_address << 1);
            v = i2c_write(0xF3);                                      // Temperature Measurement Request
            if(v) serial_send(TX, "Temperature Measurement...\n", BAUDS);
            v=0;
            while(v==0) {
                wait_ms(8);
                i2c_start();
                v = i2c_write((i2c_address << 1) | 1);                // Wait measure...
                }
            adc_t = i2c_read16() & 0xFFFC;
            i2c_stop();
            t = ((adc_t * 12500) << 16) - 600;
            sprintf(s, "temp : %i\n", t);
            serial_send(TX, s, BAUDS);
        }else{
            serial_send(PA0, "HTU21D not connected.\n", 115200);
            }
        wait_ms(1000); 
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:test_htu21d.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
