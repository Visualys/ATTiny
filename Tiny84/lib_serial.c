void serialcom_send(uint8_t TX, char* text) {		// DDRA mode
	uint8_t i = 0, pinTX = (1 << TX);
	uint16_t msg = 0;
	DDRA |= pinTX;					// set pin as output
	TCCR0A = (1<<WGM01)|(1<<WGM00);			// set FastPWM mode
	OCR0A = 5-1;					// 115200 BAUDS
	TCCR0B = 0;					// stop timer
	TCNT0 = 0;					// timer val=0
	PORTB |= pinTX;					// set pin HIGH
	TCCR0B = (1<<WGM02) | 2;			// start timer with 1/8 speed
	while((TIFR & (1<<TOV0))==0);			// idle time (high)
	TIFR |= (1<<TOV0);				// reset overflow flag
	TCNT0 = 0;
	while(text[i]) {
		msg = 0b1000000000 | (text[i] << 1);
		while(msg) {
			while((TIFR & (1<<TOV0))==0);	//wait counter 0 overflow
			if(msg & 1) {
				PORTB |= pinTX;
				}else{
				PORTB &= ~pinTX;
				}
			TIFR |= (1<<TOV0);		//reset overflow flag
			msg >>= 1;
			}
		i++;
		}
	while((TIFR & (1<<TOV0))==0);
	TCCR0B = 0;					// stop timer
	TCNT0 = 0;
	TIFR |= (1<<TOV0);				// reset overflow flag
	}
