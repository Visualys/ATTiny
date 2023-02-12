#include <avr/io.h>
#include "lib_serial.c"
#include "lib_wait.c"
#include "lib_oled.c"
#include <stdio.h>


// This Program is used to Measure SCT-013-030 on PA0.
// Returning information (UART) on PA5.
// Returning information (OLED) on PB0/PB1.

char s[50];
uint16_t i, measures;
double v;
uint8_t n;
int ent1, dec1, ent2, dec2;
float w, conso, amp;

uint16_t ADC_PA0_PA1vsVCC_read() {
    uint16_t adc;
    ADMUX = 0b10000000;                               // read PA1+/PA0- voltage vs VCC ref.
    ADCSRB = 0;
    ADCSRA = 0b10000000;                              // enable ADC
    ADCSRA |= 0b01000000;                             // start ADC
    while(ADCSRA & 0b01000000);                       // wait measure
    adc = ADCL; adc |= ADCH << 8;
    return adc;
    }

void main(void) {
    OSCCAL = 134;
    wait_ms(1000);
    oled_init(PB0, PB1);
	oled_clear(0);
	oled_cmd(0xAF);               // display ON
	oled_cmd(0x81);	oled_cmd(32); // contrast

    sprintf(s, "...");
    oled_print2(s,10,1);
    serial_send(PA5, "ATTiny84 started.\n", 115200);

    conso=0;    
    while(1){
    TCCR0A = 0b00000010;                              // CTC Mode
    OCR0A = 180;                                      // 20ms
        for(n=0;n<50;n++){
            v=0;
            measures = 0;
            TCNT0 = 0;
            TCCR0B = 0b00000101;                      // start timer0 with 1/1024 speed		
            while(!(TIFR0 & 0b00000010)){             // wait counter 0 overflow
                i = ADC_PA0_PA1vsVCC_read();
                v+=i;
                measures++;
                }
            TIFR0 |= 0b00000111;                      // reset overflow flag
            TCCR0B = 0;                               // stop timer0		
            w = (float)(v / measures) * 19.0f;
            conso += w / 180000.0f;
            }
        ent1 = w;
        dec1 = 100.0f * (w-ent1);
        ent2 = conso;
        dec2 = 100.0f * (conso-ent2);
        sprintf(s, "%i.%02d", ent1, dec1);
        oled_print2(s,8,1);
        sprintf(s, "%i.%02d", ent2, dec2);
        oled_print2(s,10,5);
        
//        sprintf(s, "Puiss.: %i.%02d w, Conso.: %i.%02d Wh\r\n", ent1, dec1, ent2, dec2);
//        serial_send(PA5, s, 115200);
        }
        
       
    }

