

#Define DEVICE_NUM 1
uint8_t REG[11];
uint8_t REG_IDX = 0; MAX_IDX = 10;
uint8_t BITS = 0;
uint8_t done=0;
DATA = 0b00000001;  // PB0 ->DATA
DDRB &= ~DATA;      // DATA as input


while(REG_IDX<MAX_IDX) {
    while(!(PINB & DATA));   // attendre un signal HIGH
    //***  Lancer le compteur (0-255) (1/64clk)
    while( (PINB & DATA) && (!(TIFR & (1<<TOV0))) ); // laisser compter jusqu'a sig=BAS ou OVF
    //***  Stopper le compteur 
    if(TIFR & (1<<TOV0)) {
        REG[0] = 0; REG_IDX = 0; BITS = 0; MAX_IDX = 10;
    }else{
	REG[REG_IDX] <<= 1;
        if(TCNT0>63) REG[0]++;
        BITS++;
        if(REG_IDX==0) {
            BITS = 99;
            if(REG[0]==0xBE) {REG_IDX++; BITS = 0;}
        }else if(BITS==8) { 
            REG_IDX++; BITS = 0;
            }
        if(BITS==0) {
            if(REG_IDX==2) {
                if(REG[1]!=DEVICE_NUM) {REG[0] = 0; REG_IDX = 0; BITS = 0; MAX_IDX = 10;}
            }else if(REG_IDX==4) { 
                MAX_IDX = 4 + (REG[3] & 7); 
                }
            }
        }      
    }


