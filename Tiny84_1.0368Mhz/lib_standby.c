#ifndef lib_standby
#define lib_standby

#include <avr/interrupt.h>
#include <avr/sleep.h>

volatile uint8_t wdcount=0;

ISR(WDT_vect) {
    wdcount++;
    WDTCSR |= (1 << WDIE);
    }

void standby_1s(uint8_t loops) {
    ADCSRA &= ~( 1 << ADEN );                                                  // set ADC off
    PRR = 0b00001011;                                                          // power off Tim1,USI,ADC
    ACSR |= (1 << ACD);
    sei();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    wdcount = 0;
    WDTCSR = (1 << WDP2) | (1 << WDP1);                                         // 1 sec
    WDTCSR |= (1 << WDIE) | (1 << WDCE) | (1 << WDE);
    while(wdcount < loops) {
        sleep_mode();
        }
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = 0;
    }

#endif
