#include <avr/io.h>
#include "lib_eeprom.c"
#include "lib/lib_wait.c"
#include "lib_str.c"
#include "lib_standby.c"

#include "lib_rf24.c"


char s[33],s1[8];
int32_t v=0;
uint16_t lev;

uint16_t read_level(){                                                          // PWR:PA5  LATCH:PA7  CLOCK:PB2  DATA:PA6
	uint16_t ret=0, ret2=0;
    uint16_t n = 32768;
    DDRA |= (1 << PA5) | (1 << PA7);                                            // PA5 output to Power ON / PA7 output to LATCH
    PORTA |= (1 << PA5);                                                        // Power ON
    DDRB |= (1 << PB2);                                                         // set clock as output
    DDRA &= ~(1 << PA6);                                                        // set data as input
    PORTA |= (1 << PA7);                                                        // set latch HIGH
    while(n){
        if(PINA & (1 << PA6)){ret |= n;};                                       // get pin value
        PORTB |= (1 << PB2);                                                    // set clock HIGH
        PORTB &= ~(1 << PB2);                                                   // set clock LOW
        n >>= 1;
        }
    PORTA &= ~(1 << PA7);                                                       // set latch LOW
    PORTA &= ~(1 << PA5);                                                       // Power OFF
    DDRA |= (1 << PA6);                                                         // kill data as input
    for(n=0;n<16;n++){
		if(ret & (1<<n)) {ret2=n+1;}    // return 1 to 16
	    }
    return ret2;
    }


void main(void) {
	OSCCAL = eeprom_read(1);                                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC

    // rf24----------------
    rf24_init(PA0,PA4,PA3,PA2,PA1);                         // ce, cs, mosi, miso, clk
    rf24_setconfig(90, 0, 3);                               // default channel
    rf24_setaddress(0, 100,100,100,100,100);                // default address
    rf24_setautoretransmit(15,15);
    rf24_set_payload_length(32);

    while(1){
        //74HC165
        lev=read_level();
        longtostr(lev, s1);
        strset(s, "event,cude=", 0);
        strset(s, s1, -1);
        strset(s, "\n", -1);
        
        // nRF24
        rf24_sendline(s);

        standby_1s(5);
        }
    }
