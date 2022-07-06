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

