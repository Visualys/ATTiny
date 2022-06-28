#include <avr/interrupt.h>
#include <avr/sleep.h>

volatile uint8_t wdcount=0;

ISR(WDT_vect) {
	wdcount++;
	WDTCR |= (1 << WDIE);
	}

void standby_min(uint8_t min) {
	ADCSRA &= ~( 1 << ADEN ); // set ADC off
	ACSR -= (1 << ACD);
	 
	sei();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	wdcount = 0;
	WDTCR = (1 << WDP3) | (1 << WDP1); // 8 sec
	WDTCR |= (1 << WDIE) | (1 << WDCE) | (1 << WDE);
	while(wdcount < (8*min)) {               // 64 sec
		sleep_mode();
		}
	WDTCR |= (1 << WDCE) | (1 << WDE);
	WDTCR = 0;
	}
