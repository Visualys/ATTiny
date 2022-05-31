// Calibrate the CPU frequency to 4 608 000 Hz

void serialcom_send(uint8_t TX, char* text, uint16_t bauds) {	// TX must be on PORTA
	uint8_t i = 0, pinTX = (1 << TX);
	uint16_t msg = 0;
	DDRA |= pinTX;						// set pin as output
	TCCR0A = (1<<WGM01)|(1<<WGM00);				// set FastPWM mode
	TCCR0B = 0;						// stop timer
	TCNT0 = 0;						// timer val=0
	if(bauds==9600){OCR0A = 59;}
	if(bauds==19200){OCR0A = 29;}
	if(bauds==115200){OCR0A = 4;}
	PORTA |= pinTX;						// set pin HIGH
	TCCR0B = (1<<WGM02) | 2;				// start timer with 1/8 speed
	while(!(TIFR & (1<<TOV0)));				// idle time (high)
	TIFR |= (1<<TOV0);					// reset overflow flag
	TCNT0 = 0;
	while(text[i]) {
		msg = 0b1000000000 | (text[i] << 1);
		while(msg) {
			while(!(TIFR & (1<<TOV0)));		//wait counter 0 overflow
			if(msg & 1) {
				PORTB |= pinTX;
				}else{
				PORTB &= ~pinTX;
				}
			TIFR |= (1<<TOV0);			//reset overflow flag
			msg >>= 1;
			}
		i++;
		}
	while(!(TIFR & (1<<TOV0)));
	TCCR0B = 0;						// stop timer
	TCNT0 = 0;
	TIFR |= (1<<TOV0);					// reset overflow flag
	}
