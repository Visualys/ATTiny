#ifndef lib_wait
#define lib_wait

void wait_ms(uint32_t msec) {
    OCR0A = 143;
    TCCR0A = (1<<WGM01)|(1<<WGM00);
    TCNT0 = 0;                             // set counter to zero
    TCCR0B = (1<<WGM02) | 3;               // start timer0 with 1/64 speed
    while(msec > 0) {
        while((TIFR0 & (1<<TOV0))==0);      // wait counter 0 overflow
        TIFR0 |= (1<<TOV0);                 // reset overflow flag
        msec--;
        }
    TCCR0B = 0;                            // stop timer0
    }

void wait_us(uint32_t usec) {
    OCR0A = 8;
    TCCR0A = (1<<WGM01)|(1<<WGM00);
    TCNT0 = 0;                             // set counter to zero
    TCCR0B = (1<<WGM02) | 1;               // start timer0 with 1/1 speed
    while(usec > 0) {
        while((TIFR0 & (1<<TOV0))==0);      // wait counter 0 overflow
        TIFR0 |= (1<<TOV0);                 // reset overflow flag
        usec--;
        }
    TCCR0B = 0;                            // stop timer0
    }

#endif
