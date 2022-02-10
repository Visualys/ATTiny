#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <string.h>


void wait_s(uint32_t sec) {
  uint32_t n, i;
  OCR0A = 255;
  TCNT0 = 0;
  TCCR0B = 2;
  for(i=0; i<500; i++) {
    while(TCNT0 <= 250);
    TCNT0 = 0;
    }
  TCCR0B = 0;
  }
	
void serialcom_send(uint8_t TX, char* text) {
  uint8_t i = 0, pinTX = (1 << TX);
  uint16_t msg = 0;
  DDRB |= pinTX;              // pin as output
  TCCR0B = 0;                 //stop timer
  TCNT0 = 0;                  // timer val=0
  OCR0A = 255;
  PORTB |= pinTX;             // pin is HIGH
  TCCR0B = 1;                 //start timer
  while(TCNT0 < 104);         // idle time (high)
  TCNT0 = 0;
  while(text[i]) {
    msg = 0b1000000000 | (text[i] << 1);
    while(msg) {
      while(TCNT0 < 104);
      TCNT0 = 0;
      if(msg & 1) {
        PORTB |= pinTX;
        }else{
        PORTB &= ~pinTX;
        }
      msg >>= 1;
      }
    i++;
    }
  while(TCNT0 < 104);
  TCCR0B = 0;                 //stop timer
  TCNT0 = 0;
  }

uint16_t readADC_PB2vs1V1() {
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
  uint16_t adc;
  wait_s(1);
  while(1) {
    adc = readADC_PB2vs1V1();
    sprintf(s,"adc = %i\r\n", adc);
    serialcom_send(PB0, s);
    wait_s(1);
    }
  }
