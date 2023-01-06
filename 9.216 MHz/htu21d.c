#include <avr/io.h>
#include "lib_eeprom.c"
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_htu21d_PORTB.c"
#include "lib_str.c"

char s[32], st[6];
uint16_t t=0;
uint8_t status=0;
   
void main(void) {
    OSCCAL = eeprom_read(1) ;                                        // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
    serial_send(PA5, "ATTiny84 started.\n", 115200);
    while(1){
		t=htu_read_temperature(PB0, PB1);
		strset(s, "htu temperature : ");
		longtostr(t,st);
		stradd(s, st);
		serial_send(PA5, s, 115200);
		serial_send(PA5, "\n", 115200);	
		
		t=htu_read_humidity(PB0, PB1);				
		strset(s, "htu humidity : ");
		longtostr(t,st);
		stradd(s, st);
		serial_send(PA5, s, 115200);
		serial_send(PA5, "\n", 115200);	
		
		status = htu_lowbatt();
		strset(s, "htu lowbatt : ");
		longtostr(status, st);
		stradd(s, st);
		serial_send(PA5, s, 115200);
		serial_send(PA5, "\n", 115200);	
		
        wait_ms(1000);	
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:htu21d.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
