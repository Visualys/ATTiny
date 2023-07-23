#ifndef lib_wait
#define lib_wait

void wait_ms(uint16_t msec) {
	if(msec>10){msec /= 10;}else{msec=1;}
    TCCR0A = 0b00000010;                                                       // CTC Mode
    TCNT0 = 0;                                                                 // set counter to zero
    OCR0A = 161;                                                               // 2 & 129 = 1.003ms
    TCCR0B = 3;                                                                // start timer0 with 1/8 speed
    while(msec--) {
        while(!(TIFR0 & 0b00000010));                                          // wait counter 0 overflow
        TIFR0 |= 0b00000111;                                                   // reset overflow flag
        }
    TCCR0B = 0;                                                                // stop timer0
    }

void wait_us(uint16_t usec) {
    TCCR0A = 0b00000010;                                                       // CTC Mode
    TCNT0 = 0;                                                                 // set counter to zero
    OCR0A = 0;
    TCCR0B = 1;                                                                // start timer0 with 1/1 speed
    while(usec) {
        while(!(TIFR0 & 0b00000010));                                          // wait counter 0 overflow
        TIFR0 |= 0b00000111;                                                   // reset overflow flag
        usec--;
        }
    TCCR0B = 0;                                                                // stop timer0
    }

#endif
