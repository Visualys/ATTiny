#ifndef lib_eeprom
#define lib_eeprom

uint8_t eeprom_read(uint8_t addr){
    while(EECR & (1<<EEPE));                                                   // wait if eeprom busy
    EEARH = 0;                                                                 // address high
    EEARL = addr;                                                              // address low
    EECR |= (1<<EERE);                                                         // start eeprom read
    return EEDR;
    }

void eeprom_write(uint8_t addr, uint8_t v){
    if(eeprom_read(addr) != v){
        while(EECR & (1<<EEPE));                                               // wait if eeprom busy
        EECR = (0<<EEPM1) | (0<<EEPM0);                                        // set eeprom programming mode
        EEARH = 0;                                                             // address high
        EEARL = addr;                                                          // address
        EEDR = v;                                                              // value
        EECR |= (1<<EEMPE);
        EECR |= (1<<EEPE);                                                     // start eeprom write
        }
    }

#endif