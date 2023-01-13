#include <avr/io.h>
//#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_oled.c"
#include "lib_str.c"
#include "lib_htu21d_PORTB.c"
#include "lib_rf24.c"

char s[32], st[6];
uint16_t t=0;
uint8_t k=0;

   
void main(void) {
    OSCCAL=139;
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    //serial_send(PA5, "ATTiny84 started.\n", 115200);
    wait_ms(500); // oled power up...
	t=oled_init(PB0, PB1);
	oled_clear(0);
	oled_cmd(0xAF);               // display ONn
	oled_cmd(0x81);	oled_cmd(32); // contrast
	rf24_init(PA0,PA4,PA3,PA2,PA1);                                  // ce, cs, mosi, miso, clk
    rf24_setconfig(90, 0, 3);
    rf24_setaddress(0, 100,100,100,100,100);
    rf24_setautoretransmit(15,15);
    rf24_set_payload_length(32);

	
    while(1){	

		k++;
		if(k==9) k=0;
		if(k==0) {
			strset(s,"event,tbur=");
			t=htu_read_temperature(PB0, PB1); longtostr(t,st); stradd(s, st);
			stradd(s,",");
			t=htu_read_humidity(PB0, PB1); longtostr(t,st); stradd(s, st);
			stradd(s,"\n");
			//serial_send(PA5, s, 115200);
 			oled_locate(0,6);
 			if(rf24_sendline(s)){
				oled_print("send : OK. ",0);}
				else{
				oled_print("send : ERR!",0);
				}
			}

		
			
		oled_gotoline(1);
		t=htu_read_temperature(PB0, PB1);
		strset(s, "  Temp : ");
		longtostr(t,st);
		st[5]=0;st[4]=st[3];st[3]=st[2];st[2]=46;
		stradd(s, st);
		stradd(s,"°C ");
		oled_print(s,0);

		oled_gotoline(3);	
		t=htu_read_humidity(PB0, PB1);
		strset(s, "   Hum : ");
		longtostr(t,st);
		st[5]=0;st[4]=st[3];st[3]=st[2];st[2]=46;
		stradd(s, st);
		stradd(s,"% ");
		oled_print(s,0);

		
		

			
        wait_ms(1000);	
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:testoled.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
