#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_str.c"
#include "lib_rf24.c"

#define TX PA6
#define BAUDS 115200
//                          Linky TTL  --> PA5
//                          TX (debug) --> PA6
//********************************
//event,tic1,000000000,000000000 0  // HC,HP
//event,tic2,0000,0              0  // W, EC
//event,ticsent                  0  // D1 Rule (teleinfo sent) : ticsent

char s[50], v[16], sHC[24], sHP[24], PTEC[2];
long HP=0, HC=0, pHP=0, pHC=0, W=0;

void serial_read_linky(uint8_t RX, char* msg) {
    uint8_t b = 0, n = 0, msgID = 0, pinRX = (1 << RX);
    DDRA &= ~pinRX;                                                                 // set pin as input
    TCCR0B = 0;                                                                     // stop timer
    TCCR0A = (1<<WGM01)|(1<<WGM00);                                                 // set FastPWM mode
    TCNT0 = 0;                                                                      // timer val=0
    OCR0A = 119;
    while ( b != 10 ) {  
        while(!(PINA & pinRX) );
        TCCR0B = 0;                                                                 // stop timer
        TIFR0 |= (1<<TOV0);                                                         // reset overflow flag
        TCNT0 = OCR0A >> 1;                                                         // position trigger to the middle
        while(PINA & pinRX);                                                        // wait start bit (HIGH to LOW)
        TCCR0B = (1<<WGM02) | 3;                                                    // start timer with 1/64 speed
        while(!(TIFR0 & (1<<TOV0)));                                                // wait counter 0 overflow	
        TIFR0 |= (1<<TOV0);                                                         // reset overflow flag
        b = 0;
        for(n=0; n<8; n++) {
            while(!(TIFR0 & (1<<TOV0)));                                            // wait counter 0 overflow			
                b >>= 1;
                if(PINA & pinRX) b += 128;                                          // store bit value
                TIFR0 |= (1<<TOV0);                                                 // reset overflow flag
                }
        msg[msgID] = b & 127;
        msgID++;
        }
    TCCR0B = 0;                                                                     // stop timer
    TCNT0 = 0;                                                                      // timer val=0
    TIFR0 |= (1<<TOV0);                                                             // reset overflow flag
    msg[msgID] = 0;
    } 


void main(void) {
    uint8_t counter=0, loop;
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                                        // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                                       // set ADC off
    PRR |= ( 1 << PRADC );                                                          // power off ADC
    serial_send(TX, "ATTiny84 started.\n", BAUDS);
    rf24_init(PA0,PA4,PA3,PA2,PA1);                                                 // ce, cs, mosi, miso, clk
    rf24_setconfig(90, 0, 3);
    rf24_setaddress(0, 100,100,100,100,100);
    rf24_setautoretransmit(15,15);
    rf24_set_payload_length(32);
    while(1){
        serial_read_linky(PA5, s);
        if(startwith(s,"HCHP")) {
            stxt(sHP, s, 5, 9);
        }else if(startwith(s,"HCHC")) {
            stxt(sHC, s, 5, 9);
        }else if(startwith(s,"PTEC")) {
            counter++;
            if(counter==20){
                stxt(PTEC, s, 6, 1);
                HC = strtolong(sHC);
                HP = strtolong(sHP);
                if(pHC==0) pHC=HC;
                if(pHP==0) pHP=HP;
                W=((HC-pHC)+(HP-pHP))*3600/32;
                if(W){
                    strset(s,"event,HC,");
                    longtostr(HC, v);
                    stradd(s, v);
                    stradd(s,"\n");
                    serial_send(TX, s, BAUDS);
                    strset(s,"event,HP,");
                    longtostr(HP, v);
                    stradd(s, v);
                    stradd(s,"\n");
                    serial_send(TX, s, BAUDS);
                    strset(s,"event,W,");
                    longtostr(W, v);
                    stradd(s, v);
                    stradd(s,"\n");
                    serial_send(TX, s, BAUDS);
                    strset(s,"event,EC,");
                    if(PTEC[0]==67){							// "C"
                        stradd(s,"1\n");	
                        }else{
                        stradd(s,"0\n");	
                        }
                    serial_send(TX, s, BAUDS);

                    // nRF24
                    strset(s,"event,tic1=");
                    longtostr(HC, v);
                    stradd(s, v);
                    stradd(s,",");
                    longtostr(HP, v);
                    stradd(s, v);
                    stradd(s,"\n");	
                    //strset(s,"Bonjour (by nRF24L01+)\n         ");		
                    rf24_powerup_tx();
                    rf24_send(s);
                    loop=1;
                    while(loop){
                        if(rf24_datasent()){
                            loop = 0;
                            serial_send(TX, "sent to nRF24.\n", BAUDS);
                            }
                        if(rf24_maxretry()){
                            loop = 0;
                            serial_send(TX, "Error : max retries !\n", BAUDS);
                            }
                        }
                    rf24_clear_status();
                    rf24_powerdown();
                    wait_ms(150);
                    // nRF24 .2
                    strset(s,"event,tic2=");
                    longtostr(W, v);
                    stradd(s, v);
                    stradd(s,",");
                    if(PTEC[0]==67){							// "C"
                        stradd(s,"1\n");	
                        }else{
                        stradd(s,"0\n");	
                        }		
                    rf24_powerup_tx();
                    rf24_send(s);
                    loop=1;
                    while(loop){
                        if(rf24_datasent()){
                            loop = 0;
                            serial_send(TX, "sent to nRF24.\n", BAUDS);
                            }
                        if(rf24_maxretry()){
                            loop = 0;
                            serial_send(TX, "Error : max retries !\n", BAUDS);
                            }
                        }
                    rf24_clear_status();
                    rf24_powerdown();
                    wait_ms(150);
                    // nRF24 .3
                    strset(s,"event,ticsent\n");
                    rf24_powerup_tx();
                    rf24_send(s);
                    loop=1;
                    while(loop){
                        if(rf24_datasent()){
                            loop = 0;
                            serial_send(TX, "sent to nRF24.\n", BAUDS);
                            }
                        if(rf24_maxretry()){
                            loop = 0;
                            serial_send(TX, "Error : max retries !\n", BAUDS);
                            }
                        }
                    rf24_clear_status();
                    rf24_powerdown();
                    wait_ms(150);
                    }
               pHC=HC;pHP=HP;counter=0;
               }
            }
        }
    }
	
	
/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:linky_read.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
