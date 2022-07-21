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

uint8_t i2c_sda, i2c_scl;

void i2c_start() {
    PORTB |= (i2c_sda | i2c_scl);              // set all HIGH
    wait_us(5);
    PORTB &= ~i2c_sda;
    wait_us(5);
    PORTB &= ~i2c_scl;
    wait_us(5);
    }

void i2c_stop() {
    PORTB &= ~(i2c_sda | i2c_scl);             // set all LOW
    wait_us(5);
    PORTB |= i2c_scl;
    wait_us(5);
    PORTB |= i2c_sda;
    wait_us(5);
    }

uint8_t i2c_write(uint8_t byte){
    uint8_t i = 128, ret = 0;
    i2c_start();
    while(i){
        if(i & byte){PORTB |= i2c_sda;}else{PORTB &= ~i2c_sda;}
        wait_us(5);
        PORTB |= i2c_scl;
        wait_us(5);
        PORTB &= ~i2c_scl;
        wait_us(5);
        i >>= 1;
        }
    DDRB &= ~i2c_sda;                         // sda as input
    PORTB |= i2c_sda;                         // pull-up resistor
    wait_us(5);
    PORTB |= i2c_scl;
    wait_us(5);
    if(PINB & i2c_sda){ret=0;}else{ret=1;}
    PORTB &= ~i2c_scl;
    wait_us(5);
    DDRB |= i2c_sda;                          // sda as output
    i2c_stop();
    return ret;
    }

uint8_t i2c_init(uint8_t sda, uint8_t scl, uint8_t address){
    i2c_sda = (1 << sda);
    i2c_scl = (1 << scl);
    DDRB |= (i2c_sda | i2c_scl);              // outputs
    PORTB |= (i2c_sda | i2c_scl);             // set all HIGH
    wait_us(5);
    return i2c_write(address << 1);
    }
