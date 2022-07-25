// lib_BME.c
#ifndef lib_BME
#define lib_BME

#include "lib_i2c.c"

int32_t BMEt_fine;
int16_t T[4], P[10], H[7];

uint8_t BME_measure(){
    i2c_write_reg(0xF2, 0b00000100);     // 8x sample
    i2c_write_reg(0xF4, 0b10010001);                                   // start measurement                                     
    while(i2c_read_reg(0xF3) & 8);                                     // BME busy
    return 1;
    }

int16_t BME_read16(uint8_t reg){
    return = i2c_read_reg(reg) | (i2c_read_reg(reg+1) << 8);
    }

int32_t BME_read32(uint8_t reg){
    return = ((i2c_read_reg(reg)) << 12) | ((i2c_read_reg(reg+1)) << 4) | ((i2c_read_reg(reg+2)) >> 4)
    }

int32_t BME_temperature () {
    int32_t adc = BME_read32(0xFA);
    int32_t var1, var2, t; 
    var1 = ((((adc>>3) - ((int32_t)((uint16_t)T[1])<<1))) * ((int32_t)T[2])) >> 11;
    var2 = ((((adc>>4) - ((int32_t)((uint16_t)T[1]))) * ((adc>>4) - ((int32_t)((uint16_t)T[1])))) >> 12);
    var2 = (var2 * ((int32_t)T[3])) >> 14;
    BMEt_fine = var1 + var2;
    return (BMEt_fine * 5 + 128) >> 8;
    }

uint32_t BME_pressure () {
    int32_t adc = BME_read32(0xF7);
    int32_t var1, var2;
    uint32_t p;
    var1 = (((int32_t)BMEt_fine)>>1) - (int32_t)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)P[6]);
    var2 = var2 + ((var1*((int32_t)P[5]))<<1);
    var2 = (var2>>2) + (((int32_t)P[4])<<16);
    var1 = (((P[3] * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)P[2]) * var1)>>1))>>18;
    var1 = ((((32768+var1))*((int32_t)((uint16_t)P[1])))>>15);
    if (var1 == 0) return 0;
    p = (((uint32_t)(((int32_t)1048576) - adc) - (var2>>12)))*3125;
    if (p < 0x80000000) p = (p << 1) / ((uint32_t)var1);
    else p = (p / (uint32_t)var1) * 2;
    var1 = (((int32_t)P[9]) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
    var2 = (((int32_t)(p>>2)) * ((int32_t)P[8]))>>13;
    p = (uint32_t)((int32_t)p + ((var1 + var2 + P[7]) >> 4));
    return p;
    }

uint32_t BME_humidity () {
    int32_t adc = BME_read16(0xFD);
    int32_t var1; 
    var1 = (BMEt_fine - ((int32_t)76800));
    var1 = (((((adc << 14) - (((int32_t)H[4]) << 20) - (((int32_t)H[5]) * var1)) + ((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)H[6])) >> 10) * (((var1 *
        ((int32_t)H[3])) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)H[2]) + 8192) >> 14));
    var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)H[1])) >> 4));
    var1 = (var1 < 0 ? 0 : var1);
    var1 = (var1 > 419430400 ? 419430400 : var1);
    return (uint32_t)((var1>>12)*25)>>8;
    }

uint8_t BME_init(uint8_t sda, uint8_t scl){
    uint8_t v,n, addr;
    v = i2c_init(sda,sda,0x76);
    if(v){
        addr = 0x88;
        for(n=1; n<4, n++){
            T[n] = BME_read16(addr);
            addr +=2;
            }
        for(n=1; n<10, n++){
            P[n] = BME_read16(addr);
            addr +=2;
            }
        H[1] = i2c_read_reg(0xA1);
        H[2] = BME_read16(0xE1);
        H[3] = i2c_read_reg(0xE3);
        H[4] = (i2c_read_reg(0xE4)<<4) | (i2c_read_reg(0xE5) & 0x0F);
        H[5] = (i2c_read_reg(0xE5)>>4) | (i2c_read_reg(0xE6)<<4);
        H[6] = i2c_read_reg(0xE7);
        }
    return v;
    }

#endif
