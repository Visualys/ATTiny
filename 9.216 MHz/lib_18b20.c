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

void ds18b20_writebyte(uint8_t pin, uint8_t b){
    uint8_t i = 1, xpin = (1 << pin);
    while(i) {
        PORTA &= ~xpin;   // Set pin LOW
        if(b & i){ wait_us(8); }else{ wait_us(80); }
        PORTA |= xpin;    // Set pin HIGH
        if(b & i){ wait_us(80); }else{ wait_us(2); }
        i <<= 1;
        }

uint8_t ds18b20_readbyte(uint8_t pin){
    uint8_t i = 1, b = 0, xpin = (1 << pin);
    while(i) {
        PORTA |= xpin;    // Set pin HIGH
        DDRA |= xpin;     // Set pin as output
        PORTA &= ~xpin;   // Set pin LOW
        wait_us(2);
        DDRA &= ~xpin;    // Set pin as input    
        wait_us(5);
        if(PINA & xpin) b |= i;
        wait_us(60);
        i <<= 1;
        }
    return b;
    }

uint8_t ds18b20_convert(uint8_t pin){
    if(!ds18b20_init(pin)) return 0;
    ds18b20_writebyte(pin, 0x44);                 // Request temperature conversion
    return 1;
    }

int16_t ds18b20_temperature(uint8_t pin){
    uint8_t i = 0, sp[9];
    if(!ds18b20_init(pin)) return 0;
    ds18b20_writebyte(pin, 0xCC);                 // Request sensors to ignore ROM matching phase    
    ds18b20_writebyte(pin, 0xBE);                 // Read data from the internal scratchpad
    for ( i = 0; i < 9; i++ )
        sp[i] = ds18b20_readbyte(pin);
    ds18b20_init(pin);
    return (int16_t)( sp[1] << 8 ) + sp[0];
    }
