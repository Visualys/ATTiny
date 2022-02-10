#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "lib_serialcom.c"


int main(void) {
	char s[5];

  DDRB = 0b00000110;
   
  
  while(1) {
	serialcom_read(PB0, s);  
	if(memcmp(s,"1\r\n",3)==0) {
      PORTB = 0b00000010;
      } else if(memcmp(s,"2\r\n",3)==0) {
	  PORTB = 0b00000100;  
      } else {
      PORTB = 0b00000000;
      }
   }
   
 }
