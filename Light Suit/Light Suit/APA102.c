/*
 * APA102.c
 *
 * Created: 10/14/2015 7:11:56 PM
 *  Author: ericm
 */ 
#include "APA102.h"


APA102_Segment start, end;

void APA102_InitStrip(APA102_Strip *strip, APA102_Segment *segments[], uint8_t count) {
	uint8_t i;
	
	APA102_InitSegment(&start, 1);
	APA102_InitSegment(&end, 1);
	
	start.data[0] = 0x00;
	start.data[1] = 0x00;
	start.data[2] = 0x00;
	start.data[3] = 0x00;
	
	end.data[0] = 0xFF;
	end.data[1] = 0x00;
	end.data[2] = 0x00;
	end.data[3] = 0x00;
	
	strip->count = count + 2;
	
	strip->segments = (APA102_Segment**)malloc(strip->count * sizeof(APA102_Segment*));
	
	for(i = 0; i < count; i++) {
		strip->segments[i+1] = segments[i];
	}
	
	strip->segments[0] = &start;
	strip->segments[strip->count - 1] = &end;
	
	SPI_Init();
}

void APA102_InitSegment(APA102_Segment *segment, const uint8_t count) {
	uint8_t i;
	
	segment->size = count * FRAME_WIDTH;
	segment->data = (uint8_t*)malloc(segment->size);
	segment->start = 0;
	
	for(i = 0; i < count; i++) {
		segment->data[4*i] = 0xFF;
		segment->data[4*i+1] = 0x00;
		segment->data[4*i+2] = 0x00;
		segment->data[4*i+3] = 0x00;
	}
}

void APA102_SetPixel(APA102_Segment *segment, uint16_t id, uint8_t r, uint8_t g, uint8_t b) {
	uint16_t actualID = id*4 + segment->start;
	
	if(actualID > segment->size)
		actualID -= segment->size;
	
	(segment->data)[actualID + 1] = b;
	(segment->data)[actualID + 2] = g;
	(segment->data)[actualID + 3] = r;
}

void APA102_SetAll(APA102_Segment *segment, uint8_t r, uint8_t g, uint8_t b) {
	uint8_t i, count = segment->size / 4;
	
	for(i = 0; i < count; i++) {
		segment->data[4*i + 1] = b;
		segment->data[4*i + 2] = g;
		segment->data[4*i + 3] = r;
	}
}

void APA102_Increment(APA102_Segment *segment, uint8_t shiftAmount) {
	segment->start += shiftAmount*4;
	
	if(segment->start >= segment->size)
		segment->start -= segment->size;
}

void APA102_Decrement(APA102_Segment *segment, uint8_t shiftAmount) {
	APA102_Increment(segment, segment->size/4 - shiftAmount);
}

void APA102_Send(APA102_Strip *strip) {
	SPI_SendSewingMachineCirc(strip->segments, strip->count);
}

void APA102_MixColor(uint8_t magnitude, uint8_t angle, uint8_t *r, uint8_t *g, uint8_t *b) {
	uint8_t reds[] = {magnitude, magnitude, 0, 0, 0, magnitude};
	uint8_t greens[] = {0, magnitude, magnitude, magnitude, 0, 0};
	uint8_t blues[] = {0, 0, 0, magnitude, magnitude, magnitude};
	
	uint8_t sextant = angle / 43, nextSextant = (sextant + 1) % 6;
	uint32_t remainder = angle % 43, nextRemainder = 43 - remainder;
	
	*r = ((uint16_t)reds[sextant]*nextRemainder + reds[nextSextant]*remainder) / 86;
	*g = ((uint16_t)greens[sextant]*nextRemainder + greens[nextSextant]*remainder) / 86;
	*b = ((uint16_t)blues[sextant]*nextRemainder + blues[nextSextant]*remainder) / 86;
}