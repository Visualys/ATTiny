#include <avr/io.h>
#include "lib_serial.c"
#include "lib_wait.c"
#include <stdio.h>


// This Program is used to Measure SCT-013-030 on PB2

char s[60];
uint16_t i, measures;
double v;
uint8_t n;
int ent1, dec1, ent2, dec2;
float w, amp;

uint16_t ADC_PA0_PA1vsVCC_read() {
	uint16_t adc;
	ADMUX = 0b10000000;		// read PA1+/PA0- voltage vs VCC ref.
	ADCSRB = 0;
	ADCSRA = 0b10000000;         // enable ADC
	ADCSRA |= 0b01000000;        // start ADC
	while(ADCSRA & 0b01000000);  // wait measure
	adc = ADCL; adc |= ADCH << 8;
	return adc;
	}



void main(void) {
	OSCCAL = 134;
    wait_ms(500);
    serial_send(PA5, "ATTiny84 started.\n", 115200);
        
    while(1){
		v=0;
		measures = 0;
        for(n=0;n<4;n++){
			// initialise Timer0
			TCCR0B = 0;                            // stop timer0
			TCNT0 = 0;
			TCCR0A = 0b00000010;         // CTC Mode
			OCR0A = 180;
			TCCR0B = 0b00000101;               // start timer0 with 1/1024 speed		
			while(!(TIFR0 & 0b00000010)){         // wait counter 0 overflow
				i = ADC_PA0_PA1vsVCC_read();
				v+=i;
				measures++;
				}
			TIFR0 |= 0b00000111;                    // reset overflow flag
			TCCR0B = 0;                            // stop timer0		
            }
		if(measures==0) measures=1;
		w = (float)(v / measures) * 19.0f;
        ent1 = w;
		dec1 = 100.0f * (w-ent1);
		if(dec1<0) dec1*=-1;

        amp = w / 230.0f;
        ent2 = amp;
		dec2 = 100.0f * (amp-ent2);
		if(dec2<0) dec2*=-1;
;
		sprintf(s, "Watts : %i.%02d (%i.%02d A)\r\n", ent1, dec1, ent2, dec2);
		serial_send(PA5, s, 115200);
        wait_ms(10);	
        }
    }





