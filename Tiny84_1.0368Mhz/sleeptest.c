#include <avr/io.h>
#include "lib_eeprom.c"
#include "lib_serial.c"
#include "lib/lib_wait.c"
#include "lib_str.c"
#include "lib_standby.c"

#include "lib/lib_htu21d.c"
#include "lib_rf24.c"

// This Program is used to Synchronize CPU to 1.0368 MHz (956ns period)

char s[40],st[6];
int32_t v=0, deg;

void toogle_pin(uint8_t portnum, uint8_t pinnum){
    volatile uint8_t *port = (void *) (59-3*portnum);                           // set port memory address
	volatile uint8_t *ddr = port-1;                                             // set ddr memory address
	uint8_t pin=(1 << pinnum);
    *ddr |= pin;                                                                // set pin as output
	*port |= pin ;                                                              // pin high
	wait_ms(200);                                                               // wait 200ms
	*port &= ~pin ;                                                             // pin low
	}


void main(void) {
	OSCCAL = eeprom_read(1);                                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA5, "ATTiny84 started.\n", 9600);
    htu_init(PB0, PB1);

    // rf24----------------
    rf24_init(PA0,PA4,PA3,PA2,PA1);                         // ce, cs, mosi, miso, clk
    rf24_setconfig(90, 0, 3);                               // default channel
    rf24_setaddress(0, 100,100,100,100,100);                // default address
    rf24_setautoretransmit(15,15);
    rf24_set_payload_length(32);





    while(1){

		//toogle_pin(0, 0); //PA0

		deg = htu_read_temperature()*100;
		longtostr(deg, st);
		strset(s, "htu temperature : ", 0);
		strset(s, st, -1);
		strset(s,"\n", -1);
		serial_send(PA5, s, 9600);

		if(htu_lowbatt()){serial_send(PA5, "low battery !\n", 9600);}
		
        v++;
		longtostr(v, s);
		serial_send(PA5, "V = ", 9600);
		serial_send(PA5, s, 9600);
		serial_send(PA5, "\n", 9600);
        standby_1s(5);
        
        
        //rf24
        strset(s, "event,toto=1\n                   ", 0);
        rf24_sendline(s);

        
        }
    }






