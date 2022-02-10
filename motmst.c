#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "lib_serialcom.c"
#include "lib_wait.c"


int main(void) {
	char s[50];

  DDRB = 0b00000110;
  wait_s(2);
  serialcom_send(PB2,"starting...\r\n");
  while(1) {
	serialcom_read(PB0, s);  
	if(memcmp(s,"OUVRE\r\n",7)==0) {
		PORTB |= (1 << PB3);     //active pull-up
		serialcom_send(PB1,"1\r\n");
		while((PINB & (1 << PB3))==0);  //attendre tant que PB3 est en contact
		serialcom_send(PB1,"0\r\n");
		PORTB &= ~(1 << PB3);    //desactive pull-up
		serialcom_send(PB2,"event,porte=1\r\n");
	} else if(memcmp(s,"FERME\r\n",7)==0) {
		PORTB |= (1 << PB4);     //active pull-up
		serialcom_send(PB1,"2\r\n");
		while((PINB & (1 << PB4))==0);  //attendre tant que PB4 est en contact
		serialcom_send(PB1,"0\r\n");
		PORTB &= ~(1 << PB4);    //desactive pull-up
		serialcom_send(PB2,"event,porte=0\r\n");
		}
	}
 }
