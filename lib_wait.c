void wait_s(uint32_t sec) {
  uint32_t n, i;
  TCCR0A = 0;
  TCCR0B = 2;
  TCNT0 = 0;
  for(n=0;n<sec;n++) {
    for(i=0; i<500; i++) {
      while(TCNT0 < 250);
      TCNT0 = 0;
      }
    }
  TCCR0B = 0;
  }
