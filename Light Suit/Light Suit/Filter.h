/*
 * Filter.h
 *
 * Created: 10/27/2015 4:37:03 PM
 *  Author: ericm
 */ 


#ifndef FILTER_H_
#define FILTER_H_

#include <avr/io.h>

void CompFilterS(int16_t *val, int16_t newVal, uint16_t strength);
void CompFilter(uint16_t *val, uint16_t newVal, uint16_t strength);

void WaterfallFilter(uint16_t *val, uint16_t newVal, uint16_t rate);

#endif /* FILTER_H_ */