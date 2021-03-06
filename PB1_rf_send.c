#include <avr/io.h>

uint8_t PB1_rf_send(char* msg) {
    uint8_t b, idx = 0;
    uint8_t LOWTIME = 32, HIGHTIME = 96;
    DDRB |= (1<<PB1);                      // set PB1 as output
    OCR0A = 128;
    OCR0B = HIGHTIME; 
    TCCR0A = (1<<WGM01)|(1<<WGM00)|(1<<COM0B1)|(1<<COM0B0);
    TCNT0 = 0;                             //set counter to zero
    TCCR0B = (1<<WGM02) | 3;               //start timer0 with 1/64 speed (8MHz-> 1000 bauds)
    // preamble
    for(b = 0; b < 7; b++) {
        while((TIFR & (1<<TOV0))==0);      //wait counter 0 overflow
        TIFR |= (1<<TOV0);                 //reset overflow flag
        }
    // message
    while(msg[idx]) {
        b=128;
        while(b) {
            if(msg[idx] & b) {
                OCR0B = LOWTIME;
                }else{
                OCR0B = HIGHTIME;
                }
            b >>= 1;
            while((TIFR & (1<<TOV0))==0);  //wait counter 0 overflow
            TIFR |= (1<<TOV0);             //reset overflow flag
            }
        idx++;
        }
    PORTB &= 0b11111101;
    TCCR0B = 0;                            //stop timer0
    }

uint8_t PB1_rf_transmit(char* msg) {
    uint8_t b, idx = 0, bytescount = 4 + (msg[3] & 7);
    uint8_t LOWTIME = 32, HIGHTIME = 96;
    DDRB |= (1<<PB1);                      // set PB1 as output
    OCR0A = 128;
    OCR0B = HIGHTIME; 
    TCCR0A = (1<<WGM01)|(1<<WGM00)|(1<<COM0B1)|(1<<COM0B0);
    TCNT0 = 0;                             //set counter to zero
    TCCR0B = (1<<WGM02) | 3;               //start timer0 with 1/64 speed (8MHz-> 1000 bauds)
    // preamble
    for(b = 0; b < 7; b++) {
        while((TIFR & (1<<TOV0))==0);      //wait counter 0 overflow
        TIFR |= (1<<TOV0);                 //reset overflow flag
        }
    // message
    while(idx!=bytescount) {
        b=128;
        while(b) {
            if(msg[idx] & b) {
                OCR0B = LOWTIME;
                }else{
                OCR0B = HIGHTIME;
                }
            b >>= 1;
            while((TIFR & (1<<TOV0))==0);  //wait counter 0 overflow
            TIFR |= (1<<TOV0);             //reset overflow flag
            }
        idx++;
        }
    PORTB &= 0b11111101;
    TCCR0B = 0;                            //stop timer0
    }



void wait_ms(uint32_t msec) {
    OCR0A = 128;
    TCCR0A = (1<<WGM01)|(1<<WGM00);
    TCNT0 = 0;                             //set counter to zero
    TCCR0B = (1<<WGM02) | 3;               //start timer0 with 1/64 speed
    while(msec > 0) {
        while((TIFR & (1<<TOV0))==0);      //wait counter 0 overflow
        TIFR |= (1<<TOV0);                 //reset overflow flag
        msec--;
        }
    TCCR0B = 0;                            //stop timer0
    }

int main(void) {
/*
    wait_ms(1000);
    while(1) {
        PB1_rf_send("Test Message\r\n"); 
        wait_ms(1000);
        }
*/

    uint8_t msg[11];
    msg[0]=0xBE;
    msg[1]=0x01;
    msg[2]=0x01;
    msg[3]=0b10001001;
    msg[4]=125;
    while(1) {
        PB1_rf_transmit(msg); 
        wait_ms(1000);

    }
