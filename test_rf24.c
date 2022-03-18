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





│             ┌─────────────┐
└───┐   │     │             │     
┌───┘   │     └─────────────┘     IRQ   MISO                  -       PB0
└───┐   │                         
┌───┘   │                         MOSI  SCK                   PB1     PB2
└───┐   │
┌───┘   │                         CSN   CE                    PB4     PB3
└───────┴───
                                  3.3V  GND                   3.3V    GND


10h 26feb . Opticien 



#define CE_PIN 3
#define CSN_PIN 4

#include "RF24.h"

RF24 radio(CE_PIN, CSN_PIN);

const byte addr[6] = "00001";
int count = 0;

void setup() {
  radio.begin();
  radio.openWritingPipe(addr);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop(void){
  radio.write( &count, sizeof(count) );
  count++;
  delay(1000);
}
