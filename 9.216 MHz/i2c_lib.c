/*

      START       1         0       STOP

       _         ____                 __
  SDA   |___    |        |____     __| 
         
       ___        _        _         __  
  SCL     |_    _| |_    _| |_     _|  |_

DEVICE ADDRESS : 
SDO--GND : 1110110 (0x76)
SDO--VCC : 1110111 (0X77)

I2C READ-WRITE : 0=Write, 1=Read

--------------------------------------------------------------------------------------
*/
#ifndef lib_i2c
#define lib_i2c

uint8_t i2c_sda, i2c_scl, i2c_address;

void i2c_start() {
    PORTB |= (i2c_sda | i2c_scl);              // set all HIGH
    wait_us(1);
    PORTB &= ~i2c_sda;
    wait_us(1);
    PORTB &= ~i2c_scl;
    wait_us(1);
    }

void i2c_stop() {
    PORTB &= ~(i2c_sda | i2c_scl);             // set all LOW
    wait_us(1);
    PORTB |= i2c_scl;
    wait_us(1);
    PORTB |= i2c_sda;
    wait_us(1);
    }

uint8_t i2c_write(uint8_t byte){
    uint8_t i = 128, ret = 0;
    while(i){
        if(i & byte){PORTB |= i2c_sda;}else{PORTB &= ~i2c_sda;}
        wait_us(1);
        PORTB |= i2c_scl;
        wait_us(1);
        PORTB &= ~i2c_scl;
        wait_us(1);
        i >>= 1;
        }
    DDRB &= ~i2c_sda;                         // sda as input
    PORTB |= i2c_sda;                         // pull-up resistor
    wait_us(1);
    PORTB |= i2c_scl;
    wait_us(1);
    if(PINB & i2c_sda){ret=0;}else{ret=1;}
    PORTB &= ~i2c_scl;
    wait_us(1);
    DDRB |= i2c_sda;                          // sda as output
    return ret;
    }
    
uint8_t i2c_read(){    
    uint8_t i = 128, ret = 0;
    DDRB &= ~i2c_sda;                         // sda as input
    PORTB |= i2c_sda;                         // pull-up resistor
    wait_us(1);
    while(i){
        PORTB |= i2c_scl;
        wait_us(1);
        if(PINB & i2c_sda){ret |=i;}
        wait_us(1);
        PORTB &= ~i2c_scl;
        wait_us(1);
        i >>= 1;
        }
    PORTB |= i2c_scl;
    wait_us(1);
                                              // NOACK
    PORTB &= ~i2c_scl;
    wait_us(1);
    DDRB |= i2c_sda;                          // sda as output
    return ret;
	}

uint8_t i2c_init(uint8_t sda, uint8_t scl, uint8_t address){
	uint8_t ret;
    i2c_sda = (1 << sda);
    i2c_scl = (1 << scl);
    i2c_address = address;
    DDRB |= (i2c_sda | i2c_scl);              // outputs
    PORTB |= (i2c_sda | i2c_scl);             // set all HIGH
    wait_us(1);
    i2c_start();
    ret = i2c_write(i2c_address << 1);
    i2c_stop();
    return ret;
    }

uint8_t i2c_write_reg(uint8_t reg, uint8_t value){
	uint8_t ret;
    i2c_start();
    ret = i2c_write(i2c_address << 1);
    if(ret==0) return 0;
    ret = i2c_write(reg);
    if(ret==0) return 0;
    ret = i2c_write(value);
    i2c_stop();    
    if(ret==0) return 0;
    return ret;
    }

uint8_t i2c_read_reg(uint8_t reg){
	uint8_t ret;
    i2c_start();
    ret = i2c_write(i2c_address << 1);
    if(ret==0) return 0;
    ret = i2c_write(reg);
    if(ret==0) return 0;
    
    i2c_start();
    ret = i2c_write((i2c_address << 1) | 1);
    
    ret = i2c_read();
    i2c_stop();    
    if(ret==0) return 0;
    return ret;
    }

#endif
