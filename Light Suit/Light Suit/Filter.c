/*
 * Filter.c
 *
 * Created: 10/27/2015 4:45:05 PM
 *  Author: ericm
 */ 

#include "Filter.h"

void CompFilterS(int16_t *val, int16_t newVal, uint16_t strength) {
	*val = (*val) * strength / 100 + newVal * (100 - strength) / 100;
}

void CompFilter(uint16_t *val, uint16_t newVal, uint16_t strength) {
	*val = (*val) * strength / 100 + newVal * (100 - strength) / 100;
}

void WaterfallFilter(uint16_t *val, uint16_t newVal, uint16_t rate) {
	if(newVal > *val) {
		*val = newVal;
	}
	else if(newVal < *val) {
		uint16_t sub = *val - newVal;
		if(sub > rate)
			sub = rate;
		
		*val -= rate;
	}
}