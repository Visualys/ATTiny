#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "lib_serialcom.c"
#include "lib_rf433.c"
#include "lib_wait.c"

int main(void) {
  char s[50];
  wait_s(1);
  //serialcom_send(PB0, "Starting SerialCom : type a character to send.\r\n");
  
  while(1) {
    serialcom_read(PB1, s);
    //serialcom_send(PB0, "recu : "); serialcom_send(PB0, s);
    rf_send(PB4,s); 
    wait_s(1);
    }
  }
