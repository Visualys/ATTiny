#include <avr/io.h>
#include "lib_eeprom.c"
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_oled.c"
#include "lib_str.c"
#include "lib_htu21d_PORTB.c"

char s[32], st[6];
uint16_t t=0;
uint8_t k=0;
   
void main(void) {
    OSCCAL = eeprom_read(1) ;                                        // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA5, "ATTiny84 started.\n", 115200);
    wait_ms(150); // oled power up...
	t=oled_init(PB0, PB1);
    while(1){

		
		//t=oled_command(); //display off
		//t=oled_write(0xAE);
		//oled_stop();
	
		if(k==0) oled_clear(0);
		k++;


		t=oled_command();
		t=oled_write(0xB1); //page
		oled_stop();
		t=oled_command();
		t=oled_write(0b00000010 );
		oled_stop();		
		t=oled_command();
		t=oled_write(0b00010000);
		oled_stop();		

		t=htu_read_temperature(PB0, PB1);
		strset(s, "  Temp : ");
		longtostr(t,st);
		st[5]=0;st[4]=st[3];st[3]=st[2];st[2]=46;
		stradd(s, st);
		stradd(s,"°C ");
		oled_print(s,0);

		t=oled_command();
		t=oled_write(0xB4); //page
		oled_stop();
		t=oled_command();
		t=oled_write(0b00000010 );
		oled_stop();		
		t=oled_command();
		t=oled_write(0b00010000);
		oled_stop();		

		t=htu_read_humidity(PB0, PB1);
		strset(s, "   Hum : ");
		longtostr(t,st);
		st[5]=0;st[4]=st[3];st[3]=st[2];st[2]=46;
		stradd(s, st);
		stradd(s,"%");
		oled_print(s,0);


		
		t=oled_command(); //display on
		t=oled_write(0xAF);
		oled_stop();


        wait_ms(1000);	
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:testoled.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
