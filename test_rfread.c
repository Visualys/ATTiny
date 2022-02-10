#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "lib_serialcom.c"
#include "lib_rf433.c"
#include "lib_wait.c"

int main(void) {
  uint8_t c = 0;
  char s[50];
  wait_s(1);
  serialcom_send(PB0, "Starting SerialCom (receptor).\r\n");
  while(1) {
    if (rf_read(PB4, s)) {
      serialcom_send(PB0, s);
      wait_s(1);
      }
    }
  }
