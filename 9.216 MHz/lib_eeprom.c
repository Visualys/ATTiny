#ifndef lib_eeprom
#define lib_eeprom

uint8_t eeprom_read(uint8_t addr){
    while(EECR & (1<<EEPE));
    EEARL = addr;
    EECR |= (1<<EERE);
    return EEDR;
    }

void eeprom_write(uint8_t addr, uint8_t v){
    while(EECR & (1<<EEPE));
    EECR = (0<<EEPM1) | (0<<EEPM0);
    EEARL = addr;
    EEDR = v;
    EECR |= (1<<EEMPE);
    EECR |= (1<<EEPE);
    }

void eeprom_update(uint8_t addr, uint8_t v){
    if(eeprom_read(addr) != v) eeprom_write(addr, v);
    }

#endif
