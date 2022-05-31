// Calibrate the CPU frequency to 4 608 000 Hz

void wait_ms(uint32_t msec) {
	OCR0A = 71;
	TCCR0A = (1<<WGM01)|(1<<WGM00);
	TCNT0 = 0;					// set counter to zero
	TCCR0B = (1<<WGM02) | 3;			// start timer0 with 1/64 speed
	while(msec > 0) {
		while((TIFR0 & (1<<TOV0))==0);		// wait counter 0 overflow
		TIFR0 |= (1<<TOV0);			// reset overflow flag
		msec--;
		}
	TCCR0B = 0;					// stop timer0
	}
