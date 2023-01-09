#ifndef lib_oled
#define lib_oled

#include "lib_wait.c"

uint8_t oled_sda, oled_scl;

void oled_start() {
    PORTB |= (oled_sda | oled_scl);              // set all HIGH
    PORTB &= ~oled_sda;
    PORTB &= ~oled_scl;
    }

void oled_stop() {
    PORTB &= ~(oled_sda | oled_scl);             // set all LOW
    PORTB |= oled_scl;
    PORTB |= oled_sda;
    }
    
uint8_t oled_write(uint8_t byte){
    uint8_t i = 128, ret = 0;
    while(i){
        if(i & byte){PORTB |= oled_sda;}else{PORTB &= ~oled_sda;}
        //wait_us(1);
        PORTB |= oled_scl;
        //wait_us(1);
        PORTB &= ~oled_scl;
        //wait_us(1);
        i >>= 1;
        }
    DDRB &= ~oled_sda;                         // sda as input
    PORTB |= oled_sda;                         // pull-up resistor
    //wait_us(1);
    PORTB |= oled_scl;
    //wait_us(1);
    if(PINB & oled_sda){ret=0;}else{ret=1;}
    PORTB &= ~oled_scl;
    //wait_us(1);
    DDRB |= oled_sda;                          // sda as output
    return ret;
    }
 
 uint8_t oled_init(uint8_t sda, uint8_t scl){
    uint8_t ret;
    oled_sda = (1 << sda);
    oled_scl = (1 << scl);
    DDRB |= (oled_sda | oled_scl);              // outputs
    PORTB |= (oled_sda | oled_scl);             // set all HIGH
    //wait_us(1);
    oled_start();
    ret = oled_write(0x78);
    oled_stop();
    return ret;
    }
 
uint8_t oled_command(){
    uint8_t ret;
    oled_start();
    ret = oled_write(0x78);
    if(ret==0) return 0;
    ret = oled_write(0x80);  // command mode
    if(ret==0) return 0;
    return 1;
    }

uint8_t oled_ram(){
    uint8_t ret;
    oled_start();
    ret = oled_write(0x78);
    if(ret==0) return 0;
    ret = oled_write(0x40);  // ram mode
    if(ret==0) return 0;
    return 1;
    }  

void oled_clear(uint8_t k){  
	uint8_t p, n; 
	for(p=0;p<8;p++){
		if(oled_command()){
			oled_write(0xB0|p);
			oled_stop();
			oled_ram();
			for(uint8_t n=0;n<133;n++){
				oled_write(k);
				}
			oled_stop();
			}
		}
	}


void oled_print(char s[], uint8_t mono){
	uint8_t i=0, c, j, t[]={0,0,0,0,0,0,0,0}, last=1;
	oled_ram();
	while(s[i]){
		c=s[i];
		if(c==0x41)	{t[0]=124;t[1]=126;t[2]=19;t[3]=19;t[4]=126;t[5]=124;t[6]=0;t[7]=0;}
		else if(c==0x42){ t[0]=65;t[1]=127;t[2]=127;t[3]=73;t[4]=73;t[5]=127;t[6]=54;t[7]=0;}	
		else if(c==0x65){ t[0]=56;t[1]=124;t[2]=84;t[3]=84;t[4]=92;t[5]=24;t[6]=0;t[7]=0;}	
		else if(c==0x69){ t[0]=0;t[1]=68;t[2]=125;t[3]=125;t[4]=64;t[5]=0;t[6]=0;t[7]=0;}	
		else if(c==0x6E){ t[0]=124;t[1]=124;t[2]=4;t[3]=4;t[4]=124;t[5]=120;t[6]=0;t[7]=0;}	
		else if(c==0x6F){ t[0]=56;t[1]=124;t[2]=68;t[3]=68;t[4]=124;t[5]=56;t[6]=0;t[7]=0;}	
		else if(c==0x74){ t[0]=0;t[1]=4;t[2]=62;t[3]=127;t[4]=68;t[5]=36;t[6]=0;t[7]=0;}	
		else{t[0]=0;t[1]=0;t[2]=0;t[3]=0;t[4]=0;t[5]=0;t[6]=0;t[7]=0;}
		for(j=0;j<8;j++){
			if(mono) {oled_write(t[j]);}
			else if(t[j]==0 && last==0){} 
			else{oled_write(t[j]); last=t[j];} 
			}
		i++;
		}
	oled_stop();
	}
#endif
