#include <avr/io.h>
#include <stdio.h>
#include "lib_wait.c"
#include "lib_serial.c"
#include "lib_rf24.c"


int main(void) {
	OSCCAL = 10;                      //110 sur l'emmeteur 111
	char s[50];
	wait_ms(1000);
	serial_send(PA6, "Test : sending from ATTiny 84 via nRF24L01+...\r\n",9600);

	rf24_init(PA0,PA1,PA2,PA3,PA4);         // ce, cs, mosi, miso, clk
	rf24_setconfig(2, 0, 0);
	rf24_setaddress(0, 100,100,100,100,100);
	rf24_setautoretransmit(15,15);
	rf24_set_payload_length(3);
	s[0]=1;
	s[1]=2;
	s[2]=3;
	s[3]=0;
uint8_t loop;
	while(1){
		rf24_powerup_tx();
		s[2]++;
		if(s[2]==0) s[2]=1;
				
		rf24_send(s);
		loop = 1;
		while(loop){
			if(rf24_datasent()){
				loop = 0;
				serial_send(PA6, "+",9600);
				}
			if(rf24_maxretry()){
				loop = 0;
				serial_send(PA6, "Max retries!\r\n",9600);
				}
			}
		rf24_clearstatus();
		rf24_powerdown();
		wait_ms(200);
		}
	}
/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:test_rf24.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
