
uint8_t rf_read(uint8_t xpin, char* msg) {
  uint8_t length, b = 128, c, pin=(1<<xpin), msgidx=0;
  DDRB &= ~pin;              // pin as input
  TCCR0A = 1;                // Timer0 Normal Mode
  OCR0A = 255;
  TCCR0B = 2;                // Timer0 start (clk/8)
  do {
    c = 0;
    while(b) {
      while(PINB & pin) {         // Pin is HIGH
        if((TCNT0 >= 104) && (b != 128)) {return 0;}
        }
      TCNT0 = 0;
      while((PINB & pin) == 0) { // Pin is LOW
        if((TCNT0 >= 104) && (b != 128)) {return 0;}
        }
      // Signal started (HIGH)
      TCNT0 = 0;
      while(PINB & pin) {
        if(TCNT0 >= 104) {return 0;}
        }
      length = TCNT0;
      if(length >= 52) { c |= b; }
      b >>= 1;
      }
    msg[msgidx] = c;
    msgidx++;
    b = 128;
    } while (c != 10);
    msg[msgidx] = 0;
    return 1;
  }
	
uint8_t rf_readbyte(uint8_t xpin) {
  uint8_t length, b = 128, c = 0, pin=(1<<xpin);
  DDRB &= ~pin;              // pin as input
  TCCR0A = 1;                // Timer0 Normal Mode
  OCR0A = 255;
  TCCR0B = 2;                // Timer0 start (clk/8)
  while(b) {
    while(PINB & pin) {         // Pin is HIGH
      if((TCNT0 >= 104) && (b != 128)) {return 0;}
      }
    TCNT0 = 0;
    while((PINB & pin) == 0) { // Pin is LOW
      if((TCNT0 >= 104) && (b != 128)) {return 0;}
      }
    // Signal started (HIGH)
    TCNT0 = 0;
    while(PINB & pin) {
      if(TCNT0 >= 104) {return 0;}
      }
    length = TCNT0;
    if(length >= 52) { c |= b; }
    b >>= 1;
    }
  return c;
  }

uint8_t rf_sendbyte(uint8_t xpin, uint8_t byte) {
  uint8_t b = 0, i = 0, pin=(1<<xpin);
  DDRB |= pin;               // pin as output
  PORTB &= ~pin;             // pin is LOW
  TCCR0A = 1;                // Timer0 Normal Mode
  OCR0A = 255;
  TCCR0B = 2;                // Timer0 start (clk/8)
  // preamble
  for(i = 0; i < 8; i++) {
    PORTB |= pin;
    TCNT0 = 0; while(TCNT0 < 130);
    PORTB &= ~pin;
    TCNT0 = 0; while(TCNT0 < 13);
    }
  
  // message
  //for(i = 0; i < 7; i++) {  
    b=128;
    while(b) {
      if(byte & b) {
        PORTB |= pin;
        TCNT0 = 0; while(TCNT0 < 78);
        PORTB &= ~pin;
        TCNT0 = 0; while(TCNT0 < 26);
        }else{
        PORTB |= pin;
        TCNT0 = 0; while(TCNT0 < 26);
        PORTB &= ~pin;
        TCNT0 = 0; while(TCNT0 < 78);
        }
      b >>= 1;
      }
    //}
  }


uint8_t rf_send(uint8_t xpin, char* msg) {
  uint8_t b = 0, i = 0, pin=(1<<xpin), idx = 0;
  DDRB |= pin;               // pin as output
  PORTB &= ~pin;             // pin is LOW
  TCCR0A = 1;                // Timer0 Normal Mode
  OCR0A = 255;
  TCCR0B = 2;                // Timer0 start (clk/8)
  // preamble
  for(i = 0; i < 8; i++) {
    PORTB |= pin;
    TCNT0 = 0; while(TCNT0 < 130);
    PORTB &= ~pin;
    TCNT0 = 0; while(TCNT0 < 13);
    }
  // message
  while(msg[idx]) {
    b=128;
    while(b) {
      if(msg[idx] & b) {
        PORTB |= pin;
        TCNT0 = 0; while(TCNT0 < 78);
        PORTB &= ~pin;
        TCNT0 = 0; while(TCNT0 < 26);
        }else{
        PORTB |= pin;
        TCNT0 = 0; while(TCNT0 < 26);
        PORTB &= ~pin;
        TCNT0 = 0; while(TCNT0 < 78);
        }
      b >>= 1;
      }
    idx++;
    }
  }
