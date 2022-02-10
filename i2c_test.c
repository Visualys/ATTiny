#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib_serialcom.c"
#include "lib_wait.c"


uint8_t pinChanged(uint8_t Pin) {
	uint8_t old = (PINB & Pin);  
	while( old == (PINB & Pin)); // on attend un changement
	return old ? 1 : 0;          // on retourne le type de changement 1:(0->1) 0:(1->0)
	}

uint8_t i2c_read(uint8_t pinSDA, uint8_t pinSCL, uint16_t valueToSend) {
	uint8_t SDA = (1 << pinSDA), SCL = (1 << pinSCL), n, b=0, adresse;
	uint16_t lux=valueToSend*1.2;
	DDRB &= ~SDA; // set SDA as input
	
	while(!(pinChanged(SDA)==0 && (PINB & SCL))); // on attend le start
	
	b=0;
	for (n=0; n<8; n++) {
		while(!(pinChanged(SCL)==1)); // on attend SCL 0->1
		b <<= 1;
		b += (PINB & SDA) ? 1 : 0;
		}
	
	adresse = b;
	
	if((adresse >> 1)==0x23) {
		while(!(pinChanged(SCL)==0)); // on attend la fin du dernier bit
		// Acknowledgement
		DDRB |= SDA; // set SDA as output
		PORTB &= ~SDA; //ACK
		while(!(pinChanged(SCL)==0)); // on attend SCL 1->0
		DDRB &= ~SDA; // set SDA as input
		} else {
		return 0;
		}
		
	if(adresse & 0b00000001) {
		b= (lux >> 8);;
		DDRB |= SDA; // set SDA as output
		for (n=0; n<8; n++) {
			if(b&128) {PORTB |= SDA;} else {PORTB &= ~SDA;}
			while(!(pinChanged(SCL)==0)); // on attend SCL 0->1
			b <<= 1;
			}
		DDRB &= ~SDA; // set SDA as input
		while(!(pinChanged(SCL)==0)); // on attend ACK
		b= lux;
		DDRB |= SDA; // set SDA as output
		for (n=0; n<8; n++) {
			if(b&128) {PORTB |= SDA;} else {PORTB &= ~SDA;}
			while(!(pinChanged(SCL)==0)); // on attend SCL 0->1
			b <<= 1;
			}
		DDRB &= ~SDA; // set SDA as input
		while(!(pinChanged(SCL)==0)); // on attend ACK
		
		return 1; //pour l'instant...
	} else {
		b=0;
		for (n=0; n<8; n++) {
			while(!(pinChanged(SCL)==1)); // on attend SCL 0->1
			b <<= 1;
			b += (PINB & SDA) ? 1 : 0;
			}

		while(!(pinChanged(SCL)==0)); // on attend la fin du dernier bit
		// Acknowledgement
		DDRB |= SDA; // set SDA as output
		PORTB &= ~SDA; //ACK
		while(!(pinChanged(SCL)==0)); // on attend SCL 1->0
		DDRB &= ~SDA; // set SDA as input

		if(b==0x65) return 2; //Change measurement time L
		if(b==0x42) return 0; //Change measurement time H
		if(b==0x13) return 0; //Continuously L-Resolution Mode
		//if(b==0x23) return 0; //One Time L-Resolution Mode
		
		while(!(pinChanged(SDA)==1 && (PINB & SCL))) // on attend le end
		return 0;
		}
	}

uint16_t ADC_PB2vs1V1_read() {
  uint16_t adc;
  ADMUX = 0b10000001;          // read ADC1 (PB2) vs 1.1V ref.
  ADCSRB = 0;
  ADCSRA = 0b10000000;         //enable ADC
  ADCSRA |= 0b01000000;        //start ADC
  while(ADCSRA & 0b01000000);  // wait measure
  adc = ADCL; adc |= ADCH << 8;
  return adc;
  }

int main(void) {
  char s[50];
  uint16_t adr;
  uint16_t adc;
  wait_s(1);
  while(1) {
	adr = i2c_read(PB3, PB4, adc);
	if (adr==2) {
		adc = ADC_PB2vs1V1_read();
	/*}else if(adr==1) {
		itoa(adc, s, 10);
		serialcom_send(PB0, "i2c = ");
		serialcom_send(PB0, s);
		serialcom_send(PB0, "\r\n");
		*/
		}
    }
  }
