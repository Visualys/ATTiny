void serial_send(uint8_t TX, char* text, uint32_t TXbauds) {	// TX must be on PORTA
    uint8_t i = 0, pinTX = (1 << TX);
    uint16_t msg = 0;
    DDRA |= pinTX;						// set pin as output
    TCCR0B = 0;						        // stop timer
    TCCR0A = (1<<WGM01)|(1<<WGM00);				// set FastPWM mode
    TCNT0 = 0;						        // timer val=0
    if(TXbauds==9600){OCR0A = 119;}                             // 9600
    else if(TXbauds==19200){OCR0A = 59;}                        // 19200
    else if(TXbauds==115200){OCR0A = 9;}                        // 115200
    PORTA |= pinTX;						// set pin HIGH
    TCCR0B = (1<<WGM02) | 2;				        // start timer with 1/8 speed
    while(!(TIFR0 & (1<<TOV0)));				// idle time (high)
    TIFR0 |= (1<<TOV0);					        // reset overflow flag
    TCNT0 = 0;
    while(text[i]) {
        msg = 0b1000000000 | (text[i] << 1);
        while(msg) {
            while(!(TIFR0 & (1<<TOV0)));                        // wait counter 0 overflow
            if(msg & 1) { PORTA |= pinTX; }else{ PORTA &= ~pinTX; }
            TIFR0 |= (1<<TOV0);                                 // reset overflow flag
            msg >>= 1;
            }
        i++;
        }
    while(!(TIFR0 & (1<<TOV0)));
    TCCR0B = 0;                                                 // stop timer
    TCNT0 = 0;
    TIFR0 |= (1<<TOV0);                                         // reset overflow flag
    }

uint8_t serial_read(uint8_t RX, char* msg, uint32_t RXbauds) {
    uint8_t b = 0, n = 0, msgID = 0, pinRX = (1 << RX);
    DDRA &= ~pinRX;                                             // set pin as input
    TCCR0B = 0;                                                 // stop timer
    TCCR0A = (1<<WGM01)|(1<<WGM00);                             // set FastPWM mode
    TCNT0 = 0;                                                  // timer val=0
    if(RXbauds==9600){OCR0A = 119;}
    else if(RXbauds==19200){OCR0A = 59;}
    else if(RXbauds==115200){OCR0A = 9;}
    while ( b != 10 ) {  
        while(!(PINA & pinRX) );
        TCCR0B = 0;                                             // stop timer
        TIFR0 |= (1<<TOV0);                                     // reset overflow flag
        TCNT0 = OCR0A >> 1;                                     // position trigger to the middle
        while(PINA & pinRX);                                    // wait start bit (HIGH to LOW)
        TCCR0B = (1<<WGM02) | 2;                                // start timer with 1/8 speed
        while(!(TIFR0 & (1<<TOV0)));                            // wait counter 0 overflow	
        TIFR0 |= (1<<TOV0);                                     // reset overflow flag
        b = 0;
        for(n=0; n<8; n++) {
            while(!(TIFR0 & (1<<TOV0)));                        // wait counter 0 overflow			
            b >>= 1;
            if(PINA & pinRX) b += 128;                          // store bit value
            TIFR0 |= (1<<TOV0);                                 // reset overflow flag
            }
        msg[msgID] = b;
        msgID++;
        }
    TCCR0B = 0;                                                 // stop timer
    TCNT0 = 0;                                                  // timer val=0
    TIFR0 |= (1<<TOV0);                                         // reset overflow flag
    msg[msgID] = 0;
    } 
 
