#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_i2c.c"
#include <stdio.h>

char s[50];
uint8_t v;
char s[50];
int16_t T[4], P[10], H[7];
int32_t adc, var1, var2, t_fine, t;
uint32_t p, h;

void main(void) {
    //eeprom_update_byte((uint8_t*) 1, 140 );                        // Set the OSCCAL memory address (line to remove when set)
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA0, "ATTiny84 started.\n", 115200);
    
    while(1){
        v=i2c_init(PB0,PB1,0x76);
        if(v){
            serial_send(PA0, "BME connected.\n", 115200);
            T[1] = i2c_read_reg(0x88) | (i2c_read_reg(0x89) << 8);
            T[2] = i2c_read_reg(0x8A) | (i2c_read_reg(0x8B) << 8);
            T[3] = i2c_read_reg(0x8C) | (i2c_read_reg(0x8D) << 8);
            P[1] = i2c_read_reg(0x8E) | (i2c_read_reg(0x8F) << 8);
            P[2] = i2c_read_reg(0x90) | (i2c_read_reg(0x91) << 8);
            P[3] = i2c_read_reg(0x92) | (i2c_read_reg(0x93) << 8);
            P[4] = i2c_read_reg(0x94) | (i2c_read_reg(0x95) << 8);
            P[5] = i2c_read_reg(0x96) | (i2c_read_reg(0x97) << 8);
            P[6] = i2c_read_reg(0x98) | (i2c_read_reg(0x99) << 8);
            P[7] = i2c_read_reg(0x9A) | (i2c_read_reg(0x9B) << 8);
            P[8] = i2c_read_reg(0x9C) | (i2c_read_reg(0x9D) << 8);
            P[9] = i2c_read_reg(0x9E) | (i2c_read_reg(0x9F) << 8);
            H[1] = i2c_read_reg(0xA1);
            H[2] = i2c_read_reg(0xE1) | (i2c_read_reg(0xE2) << 8);
            H[3] = i2c_read_reg(0xE3);
            H[4] = (i2c_read_reg(0xE4)<<4) | (i2c_read_reg(0xE5) & 0x0F);
            H[5] = (i2c_read_reg(0xE5)>>4) | (i2c_read_reg(0xE6)<<4);
            H[6] = i2c_read_reg(0xE7);

            i2c_write_reg(0xF2, 0b00000100);     // 8x sample
            i2c_write_reg(0xF4, 0b10010001);                                   // start measurement                                     
            while(i2c_read_reg(0xF3) & 8);                                     // BME busy

            adc = ((i2c_read_reg(0xFA)) << 12) | ((i2c_read_reg(0xFB)) << 4) | ((i2c_read_reg(0xFC)) >> 4);  // Temp..
            var1 = ((((adc>>3) - ((int32_t)((uint16_t)T[1])<<1))) * ((int32_t)T[2])) >> 11;
            var2 = ((((adc>>4) - ((int32_t)((uint16_t)T[1]))) * ((adc>>4) - ((int32_t)((uint16_t)T[1])))) >> 12);
            var2 = (var2 * ((int32_t)T[3])) >> 14;
            t_fine = var1 + var2;
            t = (t_fine*5+128)>>8;            
            sprintf(s, "Temperature : %i\n", t);              
            serial_send(PA0, s, 115200);
            
            adc = ((i2c_read_reg(0xF7)) << 12) | ((i2c_read_reg(0xF8)) << 4) | ((i2c_read_reg(0xF9)) >> 4);  // Press.
            var1 = (((int32_t)BME280t_fine)>>1) - (int32_t)64000;   
            var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)P[6]);
            var2 = var2 + ((var1*((int32_t)P[5]))<<1);
            var2 = (var2>>2) + (((int32_t)P[4])<<16);
            var1 = (((P[3] * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)P[2]) * var1)>>1))>>18;
            var1 = ((((32768+var1))*((int32_t)((uint16_t)P[1])))>>15);
            if (var1 == 0) return 0;  //.......
            p = (((uint32_t)(((int32_t)1048576) - adc) - (var2>>12)))*3125;
            if (p < 0x80000000) p = (p << 1) / ((uint32_t)var1);
            else p = (p / (uint32_t)var1) * 2;
            var1 = (((int32_t)P[9]) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
            var2 = (((int32_t)(p>>2)) * ((int32_t)P[8]))>>13;
            p = (uint32_t)((int32_t)p + ((var1 + var2 + P[7]) >> 4));
            sprintf(s, "Pressure : %i\n", p);              
            serial_send(PA0, s, 115200);

            adc = ((i2c_read_reg(0xFD)) << 8) | (i2c_read_reg(0xFE);                                            // Hum.
            var1 = (BME280t_fine - ((int32_t)76800));
            var1 = (((((adc << 14) - (((int32_t)H[4]) << 20) - (((int32_t)H[5]) * var1)) + ((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)H[6])) >> 10) * (((var1 *
                ((int32_t)H[3])) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)H[2]) + 8192) >> 14));
            var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)H[1])) >> 4));
            var1 = (var1 < 0 ? 0 : var1);
            var1 = (var1 > 419430400 ? 419430400 : var1);
            h = (uint32_t)((var1>>12)*25)>>8;
            sprintf(s, "Humidite : %i\n", h);              
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
