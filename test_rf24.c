#include <avr/io.h>
#include <stdio.h>
#include "lib_wait.c"
#include "lib_serialcom.c"
#include "lib_nrf24.c"


int main(void) {
	OSCCAL = 110;                      //110 sur l'emmeteur 111
	char s[50];
	wait_ms(1000);
	serialcom_send(PB0, "Test : reading nRF24 status...\r\n");

	   
	while(1){
		rf24_init(PA0,PA1,PA2,PA3,PA4);         //ce, cs, mosi, miso, clk
		rf24_setfrequency(0x0A);
		rf24_setspeedlevel(2, 0);         // 250kpbs, -18dbm
		rf24_setautoretransmit(8, 8);      // delay, count


		rf24_set_tx_address(0xe0,0xe0,0xe0,0xe0,0xe0);   // transmission
		//rf24_set_rx_address(0,0xe0,0xe0,0xe0,0xe0,0xe0);   // to recept ack
		
		rf24_powerup_tx();
		rf24_send0("ABC\0");
		//rf24_send1();		
		wait_ms(1000);
		if(rf24_reg_read(0x17) & 0b00010000){
			serialcom_send(PB0, "Message sent !\r\n");
		}else{
			serialcom_send(PB0, "error...\r\n");
			rf24_printdetails();
		}
		rf24_powerdown();
		
		wait_ms(3000);

		}
	}
/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:test_rf24.hex:i -U lfuse:w:0xE2:m
*/
