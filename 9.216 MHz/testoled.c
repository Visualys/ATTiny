#include <avr/io.h>
#include "lib_eeprom.c"
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_oled.c"
#include "lib_str.c"

char s[32], st[6];
uint16_t t=0;
uint8_t status=0;
uint8_t k=0;
   
void main(void) {
    OSCCAL = eeprom_read(1) ;                                        // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA5, "ATTiny84 started.\n", 115200);
    while(1){
		t=oled_init(PB0, PB1);
		strset(s, "oled init : ");
		longtostr(t,st);
		stradd(s, st);
		serial_send(PA5, s, 115200);
		serial_send(PA5, "\n", 115200);	
		/*
		//t=oled_command(); //display off
		//t=oled_write(0xAE);
		//oled_stop();
	

			
		//k^=0xff;
		for(uint8_t p=0;p<8;p++){
			t=oled_command();
			t=oled_write(0xB0|p);
			oled_stop();
			for(uint8_t n=0;n<133;n++){
				t=oled_ram();
				t=oled_write(k);
				oled_stop();
				}
			}
*/
		t=oled_command();
		t=oled_write(0xB0);
		oled_stop();
		t=oled_command();
		t=oled_write(0b00000010);
		oled_stop();		
		t=oled_command();
		t=oled_write(0b00010000);
		oled_stop();		
		
		t=oled_ram();
		t=oled_write(0x00);
		t=oled_write(0x7C);
		t=oled_write(0x7E);
		t=oled_write(0x13);
		t=oled_write(0x13);
		t=oled_write(0x7E);
		t=oled_write(0x7C);
		t=oled_write(0x00);


		
		t=oled_command(); //display on
		t=oled_write(0xAF);
		oled_stop();
		
						
		strset(s, "display : ");
		longtostr(t,st);
		stradd(s, st);
		serial_send(PA5, s, 115200);
		serial_send(PA5, "\n", 115200);	
        wait_ms(1000);	
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:testoled.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
