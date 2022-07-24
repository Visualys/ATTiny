#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_str.c"
#include "lib_i2c.c"
#include <stdio.h>

char s[50];
uint8_t v=0;
uint32_t l=0;

int16_t T[4];
int32_t BME280t_fine;
uint32_t adc;

uint16_t t1;
int16_t  t2, t3;
uint8_t a, b, c;
double d;

uint32_t conv_unsigned16bit(uint8_t lsb, uint8_t msb){
	uint32_t d = lsb | (msb << 8);
	return d;
	}

double conv_signed16bit(uint8_t lsb, uint8_t msb){
	double d = (int)(lsb | (msb << 8));
	if(d>32767) d-=65536;
	return d;
	}

void main(void) {
    eeprom_update_byte((uint8_t*) 1, 140 );                          // Set the OSCCAL memory address (line to remove when set)
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA0, "ATTiny84 started.\n", 115200);
    
    while(1){
		v=i2c_init(PB0,PB1,0x76);
		if(v){
			serial_send(PA0, "BME connected.\n", 115200);
			i2c_write_reg(0xF2, 0b00000100);     // 8x sample
			
			
			t1= i2c_read_reg(0x88) | (i2c_read_reg(0x89) << 8);
			t2= i2c_read_reg(0x8A) | (i2c_read_reg(0x8B) << 8);
			t3= i2c_read_reg(0x8C) | (i2c_read_reg(0x8D) << 8);
			d = t3;
			d = d + 0.25;
			v = i2c_write_reg(0xF4, 0b10010001);                                   // start measurement                                     
            while(i2c_read_reg(0xF3) & 8);                                         // BME busy
 
			
			a=i2c_read_reg(0xFA);
			b=i2c_read_reg(0xFB);
			c=i2c_read_reg(0xFC);
			         
			         
			sprintf(s, "%i %i %i %i\n", t1, t2, t3, d);               
			serial_send(PA0, s, 115200);
			strset(s,"");
			longtostr(d,s);
			stradd(s,"\n");
			serial_send(PA0, s, 115200);
			sprintf(s, "-> %i %i %i \n", a, b, c);               
			serial_send(PA0, s, 115200);
			//serial_send(PA0, ".\n", 115200);
			
			
		}else{
			serial_send(PA0, "BME not connected.\n", 115200);
			}
        wait_ms(1000);	
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:i2c_reader.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/


