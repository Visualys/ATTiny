#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "lib_serialcom.c"

//volatile char msg[50];

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



	
	
int main(void) {
  char s[50];
  wait_s(1);
  serialcom_send(PB0, "Starting SerialCom v3.0 (? for help).\r\n");
  while(1) {
    serialcom_read(PB1, s);
    if(memcmp(s,"?\r\n",3)==0) {
      serialcom_send(PB0, "*************************\r\n");
      serialcom_send(PB0, "* no help for now ! :-) *\r\n");
      serialcom_send(PB0, "*************************\r\n");
      }else{
      serialcom_send(PB0, "recu : ");
      serialcom_send(PB0, s);
      }
    }
  }
