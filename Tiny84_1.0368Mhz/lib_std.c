#ifndef lib_std
#define lib_std

#include <avr/io.h>
#include "lib_eeprom.c"

void startup(){
    ADCSRA &= ~( 1 << ADEN );                                                  // set ADC off
    PRR = 0b00001011;                                                          // power off Tim1,USI,ADC
    OSCCAL = eeprom_read(1) ;                                                  // read eeprom address 1

    }

#endif