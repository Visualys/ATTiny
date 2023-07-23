uint8_t read_74HC165(uint8_t portnum, uint8_t latchpinnum, uint8_t clockpinnum, uint8_t datapinnum){
    volatile uint8_t *port = (void *) (59-3*portnum);                           // set port memory address
    volatile uint8_t *ddr = port-1, *pin = port-2;                              // set ddr & pin memory address
    uint8_t la = (1 << latchpinnum), da = (1 << datapinnum), cl = (1 << clockpinnum), ret = 0, n = 1;
    *ddr |= la | cl;                                                            // set latch & clock as output
    *ddr &= ~da;                                                                // set data as input
    *port |= la;                                                                // set latch HIGH
    while(n){
        if(*pin & da){ret |= n};                                                // get pin value
        *port |= cl;                                                            // set clock HIGH
        *port &= ~cl;                                                           // set clock LOW
        n <<= 1;
        }
    *port &= ~la;                                                               // set latch LOW
    return ret;
    }
