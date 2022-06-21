#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_wait.c"
#include "lib_serial.c"
#include "lib_rf24.c"

#define TX PA6
#define BAUDS 115200

void main(void) {
    //eeprom_update_byte((uint8_t*) 1, 110 );                        // Set the OSCCAL memory address (line to remove when set)
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(TX, "ATTiny84 started.\n", BAUDS);

    char s[50];
    uint8_t loop;
    s[0]=48;
    s[1]=49;
    s[2]=50;
    s[3]=0;
    rf24_init(PA0,PA1,PA2,PA3,PA4);                        // ce, cs, mosi, miso, clk
    rf24_setconfig(2, 0, 0);                               // default channel
    rf24_setaddress(0, 0xe7,0xe7,0xe7,0xe7,0xe7);          // default address
    rf24_setautoretransmit(15,15);
    rf24_set_payload_length(3);
    while(1){
        rf24_powerup_tx();
        rf24_send(s);
        loop = 1;
        while(loop){
            if(rf24_datasent()){
                loop = 0;
                serial_send(TX, "Message sent.\n", BAUDS);
                }
            if(rf24_maxretry()){
                loop = 0;
                serial_send(TX, "Error : Max retries !\r\n", BAUDS);
                }
            }
        rf24_powerdown();
        wait_ms(1000);
        }
    }



/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:rf24_send.c.hex:i -U lfuse:w:0xE2:m -U hfuse:w:0xD7:m
*/
