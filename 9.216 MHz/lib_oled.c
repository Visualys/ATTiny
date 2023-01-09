#ifndef lib_oled
#define lib_oled

#include "lib_wait.c"

uint8_t oled_sda, oled_scl;

void oled_start() {
    PORTB |= (oled_sda | oled_scl);              // set all HIGH
    wait_us(2);
    PORTB &= ~oled_sda;
    wait_us(2);
    PORTB &= ~oled_scl;
    wait_us(2);
    }

void oled_stop() {
    PORTB &= ~(oled_sda | oled_scl);             // set all LOW
    wait_us(2);
    PORTB |= oled_scl;
    wait_us(2);
    PORTB |= oled_sda;
    wait_us(2);
    }
    
uint8_t oled_write(uint8_t byte){
    uint8_t i = 128, ret = 0;
    while(i){
        if(i & byte){PORTB |= oled_sda;}else{PORTB &= ~oled_sda;}
        wait_us(2);
        PORTB |= oled_scl;
        wait_us(2);
        PORTB &= ~oled_scl;
        wait_us(2);
        i >>= 1;
        }
    DDRB &= ~oled_sda;                         // sda as input
    PORTB |= oled_sda;                         // pull-up resistor
    wait_us(2);
    PORTB |= oled_scl;
    wait_us(2);
    if(PINB & oled_sda){ret=0;}else{ret=1;}
    PORTB &= ~oled_scl;
    wait_us(2);
    DDRB |= oled_sda;                          // sda as output
    return ret;
    }
 
 uint8_t oled_init(uint8_t sda, uint8_t scl){
    uint8_t ret;
    oled_sda = (1 << sda);
    oled_scl = (1 << scl);
    DDRB |= (oled_sda | oled_scl);              // outputs
    PORTB |= (oled_sda | oled_scl);             // set all HIGH
    wait_us(2);
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
#endif
