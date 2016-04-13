/*
 * APA102.h
 *
 * Created: 10/14/2015 7:11:47 PM
 *  Author: ericm
 */ 


#ifndef APA102_H_
#define APA102_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "SPI.h"

#define FRAME_WIDTH		4
#define APA102_COUNT	300

typedef SPI_RawBuffer APA102_Segment;

typedef struct {
	APA102_Segment **segments;
	uint8_t count;
} APA102_Strip;

//extern uint8_t startBuffer[], endBuffer[], colorBuffer[];
//extern SPI_RawBuffer startData, endData, colorData, *stripData[];

extern APA102_Segment start, end;

void APA102_InitStrip(APA102_Strip *strip, APA102_Segment *segments[], uint8_t count);

void APA102_InitSegment(APA102_Segment *segment, const uint8_t count);

void APA102_SetPixel(APA102_Segment *segment, uint16_t id, uint8_t r, uint8_t g, uint8_t b);
void APA102_SetAll(APA102_Segment *segment, uint8_t r, uint8_t g, uint8_t b);

void APA102_Increment(APA102_Segment *segment, uint8_t shiftAmount);
void APA102_Decrement(APA102_Segment *segment, uint8_t shiftAmount);

void APA102_Send(APA102_Strip *strip);

void APA102_MixColor(uint8_t magnitude, uint8_t angle, uint8_t *r, uint8_t *g, uint8_t *b);



#endif /* APA102_H_ */