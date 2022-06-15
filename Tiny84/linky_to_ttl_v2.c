#include <avr/io.h>

uint16_t ADC_PA0_PA1vsVCC_read() {
	uint16_t adc;
	ADMUX = 0b00101000;					// read PA1+/PA0- voltage vs VCC ref.
	ADCSRB = 0;						// unipolar
	ADCSRA = 0b10000000;					// enable ADC
	ADCSRA |= 0b01000000;					// start ADC
	while(ADCSRA & 0b01000000);				// wait measure
	adc = ADCL; adc |= ADCH << 8;
	return adc;
	}

uint16_t adc;
uint8_t maxreached=0;
uint8_t seq=0;

int main(void) {
	OSCCAL = 13;
	DDRA |= (1 << PA3); 					// PA3 --> TTL OUTPUT (TX)
	OCR0A = 59;						// 8x speed
	TCCR0A = (1<<WGM01)|(1<<WGM00);
	TCNT0 = 0;						// set counter to zero
	TCCR0B = (1<<WGM02) | 2;				
	while(1){
		while(!(TIFR0 & (1<<TOV0))) {			// wait counter 0 overflow
			adc = ADC_PA0_PA1vsVCC_read();
			if(adc>512) maxreached=1;
			}
		if(seq==0) { 					// skip  1 to 7
			if(maxreached){
				PORTA &= ~(1 << PA3);
			}else{
				PORTA |= (1 << PA3);
				}
			}
		TIFR0 |= (1<<TOV0);				// reset overflow flag
		maxreached=0;
		seq++; seq |= 7;
		}
	}



/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:linky_to_ttl_v2.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
