

void serialcom_send(uint8_t TX, char* text) {
  uint8_t i = 0, pinTX = (1 << TX);
  uint16_t msg = 0;
  DDRB |= pinTX;              // pin as output
  TCCR0A = 0;
  TCCR0B = 0;                 // stop timer
  TCNT0 = 0;                  // timer val=0
  PORTB |= pinTX;             // pin is HIGH
  TCCR0B = 1;                 // start timer
  while(TCNT0 < 104);         // idle time (high)
  TCNT0 = 0;
  while(text[i]) {
    msg = 0b1000000000 | (text[i] << 1);
    while(msg) {
      while(TCNT0 < 104);
      TCNT0 = 0;
      if(msg & 1) {
        PORTB |= pinTX;
        }else{
        PORTB &= ~pinTX;
        }
      msg >>= 1;
      }
    i++;
    }
  while(TCNT0 < 104);
  TCCR0B = 0;                 // stop timer
  TCNT0 = 0;
  }

int serialcom_read(uint8_t RX, char* msg) {
  uint8_t b = 0, n = 0, msgID = 0, pinRX = (1 << RX);
  DDRB &= ~pinRX;             // set as input
  TCCR0A = (1 << WGM01);      // Timer0 in CTC Mode
  OCR0A = 104;
  
  while ( b != 10 ) {  
    while(!(PINB & pinRX) );
    TCCR0B = 0;               // stop timer
    TCNT0 = 0;                // timer val=0
    while(PINB & pinRX);      // wait start bit
    TCCR0B = 1;               // start timer 
    while(TCNT0 < 48);
    TCNT0 = 0;                // timer val=0
    b = 0;
    for(n=0; n<8; n++) {
      while(TCNT0 < 96);
      b >>= 1;
      if( (PINB & pinRX) ) b += 128;         			 
      }
    msg[msgID] = b;
    msgID++;
    }
  TCCR0B = 0;                 // stop timer
  msg[msgID] = 0;
  } 
