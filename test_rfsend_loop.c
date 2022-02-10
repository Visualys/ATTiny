#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "lib_serialcom.c"
#include "lib_rf433.c"
#include "lib_wait.c"

int main(void) {
  uint8_t counter = 0;
  char s[50];
  wait_s(1);
  
  while(1) {
	
    sprintf(s, "counter value : %i\r\n", counter);
    //serialcom_send(PB0, s);
    rf_send(PB4,s); 
    counter++;
    wait_s(3);
    }
  }
