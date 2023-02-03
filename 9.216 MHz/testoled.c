#include <avr/io.h>
//#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_oled.c"
#include "lib_htu21d_PORTB.c"
#include "lib_rf24.c"

char s[32], st[6];
uint16_t t=0;
uint8_t k=0;

float f;
int ent1, dec1, ent2, dec2;
   
void main(void) {
    OSCCAL=140;
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    //serial_send(PA5, "ATTiny84 started.\n", 115200);
    wait_ms(500); // oled power up...
	t=oled_init(PB0, PB1);
	oled_clear(0);
	oled_cmd(0xAF);               // display ON
	oled_cmd(0x81);	oled_cmd(32); // contrast
	rf24_init(PA0,PA4,PA3,PA2,PA1);                                  // ce, cs, mosi, miso, clk
    rf24_setconfig(90, 0, 2);
    rf24_setaddress(0, 100,100,100,100,100);
    rf24_setautoretransmit(10,15);
    rf24_set_payload_length(32);

		
    while(1){	
		k++;
		if(k==19) k=0;
		
		f=htu_read_temperature(PB0, PB1);
		ent1 = f;
		dec1 = 100.0f * (f-ent1);
		if(dec1<0) dec1*=-1;
		sprintf(s, "T %d.%02d  ", ent1, dec1);
		oled_print2(s,28,1);

		f=htu_read_humidity(PB0, PB1);
		ent2 = f;
		dec2 = 100.0f * (f-ent2);
		if(dec2<0) dec2*=-1;
		sprintf(s, "H %d.%02d  ", ent2, dec2);
		oled_print2(s,28,5);
					
		if(k==0) {
			sprintf(s, "event,tt02=%d.%02d,%d.%02d\n", ent1, dec1, ent2, dec2);
 			rf24_sendline(s);
			}

        wait_ms(1000);	
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:testoled.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
