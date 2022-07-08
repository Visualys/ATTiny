#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_wait.c"
#include "lib_serial.c"
#include "lib_rf24.c"
#include "lib_str.c"

#define TX PA5
#define BAUDS 115200
uint8_t loop;
char s[50];

void main(void) {
    //eeprom_update_byte((uint8_t*) 1, 145 );                        // Set the OSCCAL memory address (line to remove when set)
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(TX, "ATTiny84 started.\n", BAUDS);

    rf24_init(PA0,PA4,PA3,PA2,PA1);                         // ce, cs, mosi, miso, clk
    rf24_setconfig(90, 0, 3);                               // default channel
    rf24_setaddress(0, 100,100,100,100,100);                // default address
    rf24_setautoretransmit(15,15);
    rf24_set_payload_length(32);

    while(1){
        loop = 1;
        rf24_powerup_rx();
        while(loop){
            if(rf24_dataready) loop=0;
            }
        rf24_get_message(s);
        rf24_powerdown();
        if(startwith(s,"event")) {
            serial_send(TX, s, BAUDS);	
            }
        } 
    }

/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:nrfgate1.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
