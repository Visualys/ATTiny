#include <avr/io.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include "lib_serial.c"
#include "lib_str.c"

char s[32], V[12], I[12], VPV[12], PPV[12], CS[12], IL[12], H19[12], H20[12], H21[12], H22[12], H23[12], HSDS[12];

void output(char* name, char* value){
	long l=0;
	char v[10];
	strset(s,"event,");	
	stradd(s, name);
	stradd(s,",");	
	l = strtoint(value);			
	ltoa(l, v, 10);
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
        if(startwith(s,"V\t")) { strset(V, s);}
        else if(startwith(s,"I\t")) { strset(I, s);}	
        else if(startwith(s,"VPV\t")) { strset(VPV, s);
        }else if(startwith(s,"PPV\t")) { strset(PPV, s);
        }else if(startwith(s,"CS\t")) { strset(CS, s);
        }else if(startwith(s,"IL\t")) { strset(IL, s);
        }else if(startwith(s,"H")) {	
            if(startwith(s,"H19\t")) { strset(H19, s);
            }else if(startwith(s,"H20\t")) { strset(H20, s);
            }else if(startwith(s,"H21\t")) { strset(H21, s);
            }else if(startwith(s,"H22\t")) { strset(H22, s);
            }else if(startwith(s,"H23\t")) { strset(H23, s);
            }else if(startwith(s,"HSDS\t")) { strset(HSDS, s);
            }
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
