#include <avr/io.h>
#include <stdio.h>
#define F_CPU 800000000UL
#include <utils/delay.h>
#include <avr/interrupt.h>

#include "lib_RF24.h"

#define BIT(x) (1 << (x))
#define SETBITS(x,y) ((x) |= (y))
#define CLEARBITS(x,y) ((x) &= (~(y)))
#define SETBIT(x,y) SETBITS((x), (BIT((y))))
#define CLEARBIT(x,y) CLEARBITS((x), (BIT((y))))

