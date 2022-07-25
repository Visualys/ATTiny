#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_i2c.c"
#include <stdio.h>

char s[50];
uint8_t v;
uint16_t dig_t1;
int16_t dig_t2, dig_t3;    // can be negative
int32_t T[4], var1, var2;


void main(void) {
    //eeprom_update_byte((uint8_t*) 1, 140 );                          // Set the OSCCAL memory address (line to remove when set)
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA0, "ATTiny84 started.\n", 115200);
    
    while(1){
        v=i2c_init(PB0,PB1,0x76);
        if(v){
            serial_send(PA0, "BME connected.\n", 115200);
            dig_t1 = i2c_read_reg(0x88) | (i2c_read_reg(0x89) << 8);
            dig_t2 = i2c_read_reg(0x8A) | (i2c_read_reg(0x8B) << 8);
            dig_t3 = i2c_read_reg(0x8C) | (i2c_read_reg(0x8D) << 8);
            T[1]=dig_t1; T[2]=dig_t2; T[3]=dig_t3;
            i2c_write_reg(0xF2, 0b00000100);     // 8x sample
            i2c_write_reg(0xF4, 0b10010001);                                   // start measurement                                     
            while(i2c_read_reg(0xF3) & 8);                                         // BME busy
            T[0] = ((i2c_read_reg(0xFA)) << 12) | ((i2c_read_reg(0xFB)) << 4) | ((i2c_read_reg(0xFC)) >> 4);
            var1 = ((T[0] >> 1) - (T[1] << 3)) * T[2];
            var2 = (((T[0] >> 4) - T[1])*((T[0] >> 4) - T[1]) >> 13) * T[3] ;
            T[0] = (var1+var2) / 419430;			
            
            sprintf(s, "Temperature : %i\n");              
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
