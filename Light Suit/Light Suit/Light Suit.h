/*
 * Light_Suit.h
 *
 * Created: 10/13/2015 4:17:54 PM
 *  Author: ericm
 */ 


#ifndef LIGHT_SUIT_H_
#define LIGHT_SUIT_H_

#include <avr/io.h>
#include "util.h"
#include "APA102.h"
#include "Filter.h"
#include <math.h>

#define SPECTRUM_COUNT		5
#define OTHER_COUNT			1
#define SEGMENT_COUNT		SPECTRUM_COUNT + 4

#define SPECTRUM_LENGTH		20
#define OTHER_LENGTH		20

#define ARM_LENGTH			27
#define LEG_LENGTH			38


extern APA102_Segment spectrum[SPECTRUM_COUNT], leftArm, rightArm, rightLeg, leftLeg;
extern APA102_Segment *segments[SEGMENT_COUNT];
extern APA102_Strip strip;


inline void FillSegmentUp(APA102_Segment *segment, uint8_t count, uint8_t r, uint8_t g, uint8_t b);
inline void FillSegmentDown(APA102_Segment *segment, uint8_t count, uint8_t r, uint8_t g, uint8_t b);
inline uint16_t LogScale(uint16_t val);
inline int16_t min(int16_t a, int16_t b);

inline int16_t max(int16_t a, int16_t b);

void inline Filter(int16_t *val, int16_t newVal, uint16_t strength);

#endif /* LIGHT SUIT_H_ */