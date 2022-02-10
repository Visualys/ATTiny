#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>

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

int main(void) {
  DDRB &= 0b11111101;
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;
  while(1) {
	TCNT0 = 0;
    TCCR0B = 2; //8x
	while(TCNT0<104);
	TCNT0 = 0;
    if( PINB & 0b00000010 ) {
      serialcom_send(PB0, "0\r\n");
      }else{
	  serialcom_send(PB0, "1\r\n");
      }
    }
  }  
