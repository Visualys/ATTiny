#include <avr/io.h>
#include <stdlib.h>
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_convertions.c"
#include "lib_rf24.c"

//********************************
//event,tic1,000000000,000000000      // HC,HP
//event,tic2,0000,0                   // W, EC
//event,ticsent                       // D1 Rule : ticsent

char s[32], v[12], sHC[20], sHP[20], PTEC[2];
long HP=0, HC=0, pHP=0, pHC=0, W=0;

int serial_read_linky(uint8_t RX, char* msg) {
	uint8_t b = 0, n = 0, msgID = 0, pinRX = (1 << RX);
	DDRA &= ~pinRX;								// set pin as input
	TCCR0B = 0;									// stop timer
	TCCR0A = (1<<WGM01)|(1<<WGM00);				// set FastPWM mode
	TCNT0 = 0;									// timer val=0
	OCR0A = 59;
  while ( b != 10 ) {  
	while(!(PINA & pinRX) );
	TCCR0B = 0;									// stop timer
	TIFR0 |= (1<<TOV0);							// reset overflow flag
	TCNT0 = OCR0A >> 1;							// position trigger to the middle
	while(PINA & pinRX);						// wait start bit (HIGH to LOW)
	TCCR0B = (1<<WGM02) | 3;					// start timer with 1/8 speed
	while(!(TIFR0 & (1<<TOV0)));				// wait counter 0 overflow	
	TIFR0 |= (1<<TOV0);							// reset overflow flag
	b = 0;
    for(n=0; n<8; n++) {
		while(!(TIFR0 & (1<<TOV0)));			// wait counter 0 overflow			
		b >>= 1;
		if(PINA & pinRX) b += 128;				// store bit value
		TIFR0 |= (1<<TOV0);						// reset overflow flag
		}
    msg[msgID] = b & 127;
    msgID++;
    }
	TCCR0B = 0;									// stop timer
	TCNT0 = 0;									// timer val=0
	TIFR0 |= (1<<TOV0);							// reset overflow flag
  msg[msgID] = 0;
  } 


int main(void) {
	uint8_t counter=0;
	uint8_t loop;
	OSCCAL = 12;
	serial_send(PA6, "Starting.\n", 9600);
	
	rf24_init(PA0,PA1,PA2,PA3,PA4);         // ce, cs, mosi, miso, clk
	rf24_setconfig(2, 0, 0);
	rf24_setaddress(0, 100,100,100,100,100);
	rf24_setautoretransmit(15,15);
	rf24_set_payload_length(3);

	rf24_powerup_tx();


while(1){
		rf24_send("ABC");
		loop = 1;
		while(loop){
			if(rf24_datasent()){
				loop = 0;
				serial_send(PA6, "message sent\r\n",9600);
			}else if(rf24_maxretry()){
				loop = 0;
				serial_send(PA6, "Max retries!\r\n",9600);
				
				}
			}
		//rf24_clearstatus();
		//rf24_reg_print(5);
		rf24_powerdown();
		wait_ms(1000);
		rf24_powerup_tx();
		
		}
	

	while(1){
		serial_read_linky(PA5, s);
		if(startwith(s,"HCHP")) {
			sHP[0]=0;
			stradd(sHP, s);
		}else if(startwith(s,"HCHC")) {
			sHC[0]=0;
			stradd(sHC, s);
		}else if(startwith(s,"PTEC")) {



		rf24_powerup_tx();
		rf24_send("Bonjour\n");
		loop=1;
		while(loop){
			if(rf24_datasent()){
				loop = 0;
				serial_send(PA6, "sent\n", 9600);
				}
			if(rf24_maxretry()){
				loop = 0;
				serial_send(PA6, "Max retries!\n",9600);
				}
			}
		rf24_powerdown();



			counter++;
			if(counter==20){
				PTEC[0]=s[6];
				PTEC[1]=0;
				HC = strtoint(sHC);
				if(pHC==0) pHC=HC;
				HP = strtoint(sHP);
				if(pHP==0) pHP=HP;
				W=((HC-pHC)+(HP-pHP))*3600/32;
				if(W){
					s[0]=0;	
					stradd(s,"taskvalueset,HC,1,");				
					ltoa(HC, v, 10);
					stradd(s, v);
					stradd(s,"\n");
					serial_send(PA6, s, 9600);
					
					s[0]=0;	
					stradd(s,"taskvalueset,HP,1,");				
					ltoa(HP, v, 10);
					stradd(s, v);
					stradd(s,"\n");
					serial_send(PA6, s, 9600);

					s[0]=0;	
					stradd(s,"taskvalueset,W,1,");	
					ltoa(W, v, 10);			
					stradd(s, v);
					stradd(s,"\n");
					serial_send(PA6, s, 9600);
					 
					s[0]=0;	
					stradd(s,"taskvalueset,EC,1,");	
					if(PTEC[0]==67){							// "C"
						stradd(s,"1\n");	
						}else{
						stradd(s,"0\n");	
						}
					serial_send(PA6, s, 9600);
					serial_send(PA6, "event,sendit\n", 9600);
					
					// nRF24
					s[0]=0;
					stradd(s,"event,tic1,");
					ltoa(HC, v, 10);
					stradd(s, v);
					stradd(s,",");
					ltoa(HP, v, 10);
					stradd(s, v);
					stradd(s,"\n");
					rf24_powerup_tx();
					rf24_send(s);
					while(rf24_datasent() || rf24_maxretry()); //wait sending
					rf24_powerdown();
					}

				pHC=HC;pHP=HP;counter=0;
				}
			}
		}
	}
	
	
/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:linky_read.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
