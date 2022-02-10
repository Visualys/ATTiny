#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "lib_serialcom.c"
#include "lib_wait.c"
#include "lib_standby.c"

int main(void) {
  wait_s(2);
  
  while(1) {
	serialcom_send(PB0, "activation 1 minute.\r\n");
	wait_s(60);
	serialcom_send(PB0, "standby 5 minute.\r\n");
	standby_min(5);
    }
  }
