#include <avr/io.h>
#include "lib_wait.c"
#include "lib_serial.c"
#include "lib_str.c"

int main(void) {
    char s[50], v[10];
    long n=0;
    while(1) {
        OSCCAL = n;
        strset(s, "Valeur de OSCCAL : ");
        ltoa(n, v, 10);
        stradd(s, v);
        stradd(s, "\n");
        serial_send(PA0, s, 115200);
        wait_ms(50);
        n = (n + 1) & 0xFF;
        }
    }


/*
avrdude -p ATtiny84 -c stk500 -P /dev/ttyACM0 -U flash:w:scan_osccal.hex:i -U lfuse:w:0xA2:m -U hfuse:w:0xD7:m
*/
