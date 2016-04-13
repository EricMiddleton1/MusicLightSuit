#ifndef UTIL_H

#define UTIL_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define NULL	0

void Util_Init();
void Util_Close();

unsigned long Util_GetMillis();
unsigned long Util_GetMicros();

void Util_WaitMillis(unsigned int millis);
void Util_WaitMicros(unsigned int micros);

void inline Util_Filter(uint16_t *val, uint16_t newVal, uint16_t strength);
void inline Util_FilterSigned(int16_t *val, int16_t newVal, uint16_t strength);

extern volatile unsigned long ms;
extern volatile unsigned char adcChannel;
extern volatile unsigned int adc[5], old[5], derivative[5], offset[5];
extern volatile uint32_t scale[5];

#endif