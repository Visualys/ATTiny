uint8_t ds18b20_init(uint8_t pin){
    uint8_t ret=0, xpin = (1 << pin);
    DDRA |= xpin;     // Set pin as output
    PORTA |= xpin;    // Set pin HIGH
    wait_us(5);
    PORTA &= ~xpin;   // Set pin LOW
    wait_us(600); 
    DDRA &= ~xpin;    // Set pin as input
    wait_us(70);   
    if(PINA & xpin) ret = 1;
    wait_us(200);
    PORTA |= xpin;    // Set pin HIGH
    DDRA |= xpin;     // Set pin as output
    wait_us(600);
    return ret;
    }

void ds18b20_writebit(uint8_t x){
    PORTA &= ~xpin;   // Set pin LOW
    if(x){ wait_us(8); }else{ wait_us(80); }
    PORTA |= xpin;    // Set pin HIGH
    if(x){ wait_us(80); }else{ wait_us(2); }
    }

void ds18b20_writebyte(uint8_t b){
    uint8_t i = 1;
    while(i) {
        PORTA &= ~xpin;   // Set pin LOW
        if(b & i){ wait_us(8); }else{ wait_us(80); }
        PORTA |= xpin;    // Set pin HIGH
        if(b & i){ wait_us(80); }else{ wait_us(2); }
        i <<= 1;
        }

uint8_t ds18b20_readbit(){
    uint8_t x = 0;
    PORTA &= ~xpin;   // Set pin LOW
    wait_us(2);
    DDRA &= ~xpin;    // Set pin as input    
    wait_us(5);
    if(PINA & xpin) x = 1;
    wait_us(60);
    PORTA |= xpin;    // Set pin HIGH
    DDRA |= xpin;     // Set pin as output
    return x;
    }
    
uint8_t ds18b20_readbyte(){
    uint8_t i = 1, b = 0;
    while(i) {
        PORTA &= ~xpin;   // Set pin LOW
        wait_us(2);
        DDRA &= ~xpin;    // Set pin as input    
        wait_us(5);
        if(PINA & xpin) b |= i;
        wait_us(60);
        PORTA |= xpin;    // Set pin HIGH
        DDRA |= xpin;     // Set pin as output
        i <<= 1;
        }
    return b;
    }
