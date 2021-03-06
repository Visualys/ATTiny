#include <avr/io.h>

#define DEVICE_NUM 1


int main(void) {
    DDRB = 0b00000010;                  //PB1 as output
	uint8_t REG[11];
	uint8_t REG_IDX = 0, MAX_IDX = 10;
	uint8_t BITS = 0;
	uint8_t done=0;
	uint8_t DATA = 0b00000001;          // PB0 ->DATA
	DDRB &= ~DATA;                      // DATA as input

    TCCR0A = 0;
while(1){

    REG_IDX = 0; REG[0]=0; BITS = 0; MAX_IDX = 10;
    
	while(REG_IDX<MAX_IDX) {
		TCNT0 = 0;               // compteur à zero
		while(!(PINB & DATA));   // attendre un signal HIGH
		TCCR0B = 3;              //***  Lancer le compteur (0-255) (1/64clk)
		while( (PINB & DATA) && (!(TIFR & (1<<TOV0))) ); // laisser compter jusqu'a sig=BAS ou OVF
		TCCR0B = 0;              //***  Stopper le compteur 
		if(TIFR & (1<<TOV0)) {
			TIFR |= (1<<TOV0);                 //reset overflow flag
			REG[0] = 0; REG_IDX = 0; BITS = 0; MAX_IDX = 10;
		}else{
			REG[REG_IDX] <<= 1;
			if(TCNT0>63) REG[REG_IDX]++;
			BITS++;
			if(REG_IDX==0) {
				BITS = 99;
				if(REG[0]==0xBE) {REG_IDX++; BITS = 0;}
			    }

			if(BITS==8) { 
				if(REG_IDX==1) { if(REG[1]!=DEVICE_NUM) {REG[0] = 0; REG_IDX = 0; BITS = 0; MAX_IDX = 10; }}
				if(REG_IDX==4) { MAX_IDX = 4 + (REG[3] & 7); }
				REG_IDX++; BITS = 0;
				}

			}      
		}
	if(REG[1]==1) PORTB ^= 0b00000010;
	}
}

// avrdude -p ATtiny45 -c stk500 -P /dev/ttyACM0 -U flash:w:PB0_rf_receive.hex:i -U lfuse:w:0xE2:m
