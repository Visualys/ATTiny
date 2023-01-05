#ifndef lib_htu21d_PORTB
#define lib_htu21d_PORTB

#include "lib_wait.c"

uint8_t htu_sda, htu_scl;

void htu_start() {
    PORTB |= (htu_sda | htu_scl);              // set all HIGH
    wait_us(2);
    PORTB &= ~htu_sda;
    wait_us(2);
    PORTB &= ~htu_scl;
    wait_us(2);
    }

void htu_stop() {
    PORTB &= ~(htu_sda | htu_scl);             // set all LOW
    wait_us(2);
    PORTB |= htu_scl;
    wait_us(2);
    PORTB |= htu_sda;
    wait_us(2);
    }

uint8_t htu_write(uint8_t byte){
    uint8_t i = 128, ret = 0;
    while(i){
        if(i & byte){PORTB |= htu_sda;}else{PORTB &= ~htu_sda;}
        wait_us(2);
        PORTB |= htu_scl;
        wait_us(2);
        PORTB &= ~htu_scl;
        wait_us(2);
        i >>= 1;
        }
    DDRB &= ~htu_sda;                         // sda as input
    PORTB |= htu_sda;                         // pull-up resistor
    wait_us(2);
    PORTB |= htu_scl;
    wait_us(2);
    if(PINB & htu_sda){ret=0;}else{ret=1;}
    PORTB &= ~htu_scl;
    wait_us(2);
    DDRB |= htu_sda;                          // sda as output
    return ret;
    }

uint8_t htu_init(uint8_t sda, uint8_t scl){
    uint8_t ret;
    htu_sda = (1 << sda);
    htu_scl = (1 << scl);
    DDRB |= (htu_sda | htu_scl);              // outputs
    PORTB |= (htu_sda | htu_scl);             // set all HIGH
    wait_us(2);
    htu_start();
    ret = htu_write(0x80);
    htu_stop();
    return ret;
    }

uint8_t htu_command(uint8_t cmd){
    uint8_t ret;
    htu_start();
    ret = htu_write(0x80);
    if(ret==0) return 0;
    ret = htu_write(cmd);
    if(ret==0) return 0;
    return 1;
    }

uint8_t htu_read8(uint8_t ack){    
    uint8_t i = 128, ret = 0;
    DDRB &= ~htu_sda;                         // sda as input
    PORTB |= htu_sda;                         // pull-up resistor
    wait_us(2);
    while(i){
        PORTB |= htu_scl;
        wait_us(2);
        if(PINB & htu_sda){ret |=i;}
        wait_us(2);
        PORTB &= ~htu_scl;
        wait_us(2);
        i >>= 1;
        }
    if(ack){
        DDRB |= htu_sda;                      // sda as output    
        PORTB &= ~htu_sda;                    // ACK
        wait_us(2);
        }
    PORTB |= htu_scl;
    wait_us(2);
    PORTB &= ~htu_scl;
    wait_us(2);
    DDRB |= htu_sda;                          // sda as output
    return ret;
	}

uint8_t htu_status(){
    uint8_t ret;
    if(htu_command(0xE7)==0) return 0;
    htu_start();
    if(htu_write(0x81)==0) return 0;
    ret = htu_read8(0);
    htu_stop();
    return ret;
    }

uint16_t htu_read3B(){
    uint8_t ret=0, ret2=0, chk=0;
    while(ret==0){
        htu_start();
        ret = htu_write(0x81);
        }
    ret = htu_read8(1);
    if(ret==0) return 0;
    ret2 = htu_read8(1);
    chk = htu_read8(1);
    htu_stop();
    return (ret << 8) | (ret2 & 0xFC);
    }

uint16_t htu_read_temperature(uint8_t da, uint8_t cl){
    if(htu_init(da, cl)==0) return 0;
    if(htu_command(0xF3)==0) return 0;
    return 0.2681274414 * htu_read3B() - 4685;
    }
	
uint16_t htu_read_humidity(uint8_t da, uint8_t cl){
    if(htu_init(da, cl)==0) return 0;
    if(htu_command(0xF5)==0) return 0;
    return 0.1907348633 * htu_read3B() - 600;
    }

#endif
