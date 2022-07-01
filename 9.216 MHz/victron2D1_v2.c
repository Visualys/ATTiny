#include <avr/io.h>
#include <avr/eeprom.h>
#include "lib_serial.c"
#include "lib_str.c"

//  PA0 <-- Victron TX
//  PA1 --> D1 RX

char s[32], V[12], I[12], VPV[12], PPV[12], CS[12], IL[12], H19[12], H20[12], H21[12], H22[12], H23[12], HSDS[12];

void output(char* name, char* value){
	uint32_t l=0;
	char v[12];
	strset(s,"event,");	
	stradd(s, name);
	stradd(s,"=");	
	l = strtolong(value);
	longtostr(l, v);
	stradd(s, v);
	stradd(s,"\n");
	serial_send(PA1, s, 115200);
	}

void main(void) {
    //eeprom_update_byte((uint8_t*) 1, 161 );                        // Set the OSCCAL memory address (line to remove when set)
    OSCCAL = eeprom_read_byte((uint8_t*)1) ;                         // read eeprom address 0x01
    ADCSRA &= ~( 1 << ADEN );                                        // set ADC off
    PRR |= ( 1 << PRADC );                                           // power off ADC
	serial_send(PA1, "ATTiny Started.\n", 115200);
    while(1) {
        serial_read(PA0, s, 19200);
        if(startwith(s,"V\t")) { stxt(V, s, 2, 10);}
        else if(startwith(s,"I\t")) { stxt(I,  s,2, 10);}	
        else if(startwith(s,"VPV\t")) { stxt(VPV, s, 4, 10);}
        else if(startwith(s,"PPV\t")) { stxt(PPV, s, 4, 10);}
        else if(startwith(s,"CS\t")) { stxt(CS, s, 3, 10);}
        else if(startwith(s,"IL\t")) { stxt(IL, s, 3, 10);}
        else if(startwith(s,"H")) {	
            if(startwith(s,"H19\t")) { stxt(H19, s, 4, 10);}
            else if(startwith(s,"H20\t")) { stxt(H20, s, 4, 10);}
            else if(startwith(s,"H21\t")) { stxt(H21, s, 4, 10);}
            else if(startwith(s,"H22\t")) { stxt(H22, s, 4, 10);}
            else if(startwith(s,"H23\t")) { stxt(H23, s, 4, 10);}
            else if(startwith(s,"HSDS\t")) { stxt(HSDS, s, 5, 10);}
            
        }else if(startwith(s,"Checksum")) {
            output("V", V);
            output("I", I);
            output("VPV", VPV);
            output("PPV", PPV);
            output("CS", CS);
            output("IL", IL);
            output("H19", H19);
            output("H20", H20);
            output("H21", H21);
            output("H22", H22);
            output("H23", H23);
            output("HSDS", HSDS);
            serial_send(PA1, "event,processdone\n", 115200);
            }
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:victron2D1_v2.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
