/*
 * Light_Suit.c
 *
 * Created: 10/13/2015 4:16:06 PM
 *  Author: ericm
 */ 

#include "Light Suit.h"

void DrawLetter(uint16_t *letter, APA102_Segment *levels, uint8_t r, uint8_t g, uint8_t b) {
	uint8_t i;
	
	for(i = 0; i < 16; i++) {
		uint16_t mask = (1 << i);
		uint8_t temp;
		
		temp = !!(letter[0] & mask);
		APA102_SetPixel(&(levels[0]), 19 - i, r*temp, g*temp, b*temp);
		
		temp = !!(letter[1] & mask);
		APA102_SetPixel(&(levels[1]), i, r*temp, g*temp, b*temp);
		
		temp = !!(letter[2] & mask);
		APA102_SetPixel(&(levels[2]), 19 - i, r*temp, g*temp, b*temp);
		
		temp = !!(letter[3] & mask);
		APA102_SetPixel(&(levels[3]), i, r*temp, g*temp, b*temp);
		
		temp = !!(letter[4] & mask);
		APA102_SetPixel(&(levels[4]), 19 - i, r*temp, g*temp, b*temp);
	}
}

int main(void)
{
	APA102_Segment spectrum[SPECTRUM_COUNT], leftArm, rightArm, rightLeg, leftLeg;
	APA102_Segment *segments[SEGMENT_COUNT];
	APA102_Strip strip;
	
	uint16_t I_LED[5] = {0x8001L, 0x8001L, 0xFFFFL, 0x8001L, 0x8001L},
			S_LED[5] = {0xFF01, 0x8101, 0x8101, 0x8101, 0x81FF},
			U_LED[5] = {0xFFFF, 0x0001, 0x0001, 0x0001, 0xFFFF};
	
	int i, tick, letterPtr = 0;
	uint16_t levels[5], lastLevel[5];
	int16_t r, g, b;
	uint16_t lastMode = -1;
	
	Util_Init();
	
	DDRD &= ~(0b10000000);
	DDRB &= ~(0x02 | 0x04);
	
	PORTD |= 0b10000000;
	PORTB |= 0x02 | 0x04;
	
	for(i = 0; i < SPECTRUM_COUNT; i++) {
		APA102_InitSegment(&(spectrum[i]), SPECTRUM_LENGTH);
		segments[i] = &(spectrum[i]);
	}
	
	APA102_InitSegment(&leftArm, ARM_LENGTH);
	APA102_InitSegment(&rightArm, ARM_LENGTH);
	APA102_InitSegment(&rightLeg, LEG_LENGTH);
	APA102_InitSegment(&leftLeg, LEG_LENGTH);
	
	segments[SPECTRUM_COUNT] = &leftArm;
	segments[SPECTRUM_COUNT + 1] = &rightArm;
	segments[SPECTRUM_COUNT + 2] = &rightLeg;
	segments[SPECTRUM_COUNT + 3] = &leftLeg;
	
	APA102_InitStrip(&strip, segments, SEGMENT_COUNT);
	
	tick = 0;
	
	while(1) {
		uint8_t highestLevel, scale, lowestLevel, mode = 0;
		uint32_t letterTime;
		int16_t temp, raw[5];
		//uint8_t levels[5];
		int16_t dLevels[5];
		
		//if(! (PIND & 0b10000000) )
			//mode = 1;
		//else if(! (PINB & 0x02) )
			//mode = 2;
		//else
			//mode = 0;
		
		mode = ((!(PIND & 0b10000000))) | ((!(PINB & 0x02)) << 1) | ((!(PINB & 0x04)) << 2);
		
		//levels[0] = min(10*SPECTRUM_LENGTH, LogScale(adc[0]));
		//levels[1] = min(10*SPECTRUM_LENGTH, LogScale(adc[1]));
		//levels[2] = min(10*SPECTRUM_LENGTH, LogScale(adc[2]));
		//levels[3] = min(10*SPECTRUM_LENGTH, LogScale(adc[3]));
		//levels[4] = min(10*SPECTRUM_LENGTH, LogScale(adc[4]));
		
		raw[0] = min(10*SPECTRUM_LENGTH, LogScale(adc[0]));
		levels[0] = max(raw[0], levels[0] - 1);
		
		raw[1] = min(10*SPECTRUM_LENGTH, LogScale(adc[1]));
		levels[1] = max(raw[1], levels[1] - 1);
		
		raw[2] = min(10*SPECTRUM_LENGTH, LogScale(adc[2]));
		levels[2] = max(raw[2], levels[2] - 1);
		
		raw[3] = min(10*SPECTRUM_LENGTH, LogScale(adc[3]));
		levels[3] = max(raw[3], levels[3] - 1);
		
		raw[4] = min(10*SPECTRUM_LENGTH, LogScale(adc[4]));
		levels[4] = max(raw[4], levels[4] - 1);
		
		temp = levels[0] - lastLevel[0];
		dLevels[0] = max(temp, dLevels[0] - 5);
		
		temp = levels[1] - lastLevel[1];
		dLevels[1] = max(temp, dLevels[1] - 10);
		
		temp = levels[2] - lastLevel[2];
		dLevels[2] = max(temp, dLevels[2] - 10);
		
		temp = levels[3] - lastLevel[3];
		dLevels[3] = max(temp, dLevels[3] - 10);
		
		temp = levels[4] - lastLevel[4];
		dLevels[4] = max(temp, dLevels[4] - 10);
		
		if(dLevels[0] < 0)
			dLevels[0] = 0;
			
		if(dLevels[1] < 0)
			dLevels[1] = 0;
			
		if(dLevels[2] < 0)
			dLevels[2] = 0;
			
		if(dLevels[3] < 0)
			dLevels[3] = 0;
		
		if(dLevels[4] < 0)
			dLevels[4] = 0;
		
		lastLevel[0] = levels[0];
		lastLevel[1] = levels[1];
		lastLevel[2] = levels[2];
		lastLevel[3] = levels[3];
		lastLevel[4] = levels[4];
		
		//temp = min(SPECTRUM_LENGTH, levels[0] / 10);
		

		
			
		temp = (raw[0] + raw[1])/10 + 40*dLevels[0];
		Filter(&r, temp, 30);
			
		temp = (raw[1] + raw[2] + raw[3])/10 - 40*dLevels[0] - 20*dLevels[4] - 20*dLevels[3];
		Filter(&g, temp, 30);
			
		temp = ((raw[3] >> 1) + raw[4])/10 - 40*dLevels[0] + 20*dLevels[4] + 20*dLevels[3];
		Filter(&b, temp, 30);
			
		//r = 100*dLevels[0];
		//g = 0;
		//b = raw[3] + raw[4] - 100*dLevels[0];
			
		//r = 10*dLevels[0];
		//g = 0;
		//b = 0;
			
		if(r < 0)
			r = 0;
		if(g < 0)
			g = 0;
		if(b < 0)
			b = 0;
			
		highestLevel = max(r, max(g, b));
		lowestLevel = min(r, min(g, b));
			
		scale = max(highestLevel / 255, 1);
			
		//r /= scale;
		//g /= scale;
		//b /= scale;
		
		if(mode == 1) {
			FillSegmentDown(&(spectrum[0]), levels[0]/10, 255, 0, 0);
			FillSegmentUp(&(spectrum[1]), levels[1]/10, 255, 255, 0);
			FillSegmentDown(&(spectrum[2]), levels[2]/10, 0, 255, 0);
			FillSegmentUp(&(spectrum[3]), levels[3]/10, 0, 255, 127);
			FillSegmentDown(&(spectrum[4]), levels[4]/10, 0, 0, 255);
			
			tick++;
			
			if(tick >= 4) {
				tick = 0;
				
				APA102_Decrement(&leftArm, 1);
				APA102_Decrement(&rightArm, 1);
				APA102_Decrement(&rightLeg, 1);
				APA102_Decrement(&leftLeg, 1);
				
				APA102_SetPixel(&leftArm, 0, r / scale, g / scale, b / scale);
				APA102_SetPixel(&rightArm, 0, r / scale, g / scale, b / scale);
				APA102_SetPixel(&rightLeg, 0, r / scale, g / scale, b / scale);
				APA102_SetPixel(&leftLeg, 0, r / scale, g / scale, b / scale);
			}
			APA102_Send(&strip);
		}
		if(mode != lastMode || mode == 1 || mode > 3) {
			
			
			if(mode == 2 || mode == 3) {
				uint8_t brightness;
				
				if(mode == 2)
					brightness = 32;
				else
					brightness = 127;
				/*for(i = 0; i < SPECTRUM_COUNT; i++) {
					APA102_SetAll(&(spectrum[i]), 0, 0, 0);
				}*/
				FillSegmentDown(&(spectrum[0]), 13, brightness, brightness, 0);
				FillSegmentUp(&(spectrum[1]), 15, brightness, brightness, 0);
				FillSegmentDown(&(spectrum[2]), 17, brightness, brightness, 0);
				FillSegmentUp(&(spectrum[3]), 15, brightness, brightness, 0);
				FillSegmentDown(&(spectrum[4]), 13, brightness, brightness, 0);
			
				APA102_SetAll(&leftArm, brightness << 1, 0, 0);
				APA102_SetAll(&rightArm, brightness << 1, 0, 0);
				APA102_SetAll(&leftLeg, brightness, brightness, 0);
				APA102_SetAll(&rightLeg, brightness, brightness, 0);
				
				
			}
			else if(mode > 3) {
				if(lastMode != mode) {
					letterPtr = 0;
					letterTime = 0;//Util_GetMillis();;
				}
				
				if(Util_GetMillis() >= (letterTime)) {
					letterTime = Util_GetMillis() + 500;
				
					uint16_t *letter;
					uint8_t brightness;
					
					if(mode == 4)
						brightness = 32;
					else
						brightness = 127;
					
					if(letterPtr == 0)
						letter = I_LED;
					else if(letterPtr == 1)
						letter = S_LED;
					else if(letterPtr == 2)
						letter = U_LED;
					
					letterPtr++;
					if(letterPtr >= 3)
					letterPtr = 0;
					
					DrawLetter(letter, spectrum, brightness, brightness, 0);
					
					APA102_SetAll(&leftArm, brightness << 1, 0, 0);
					APA102_SetAll(&rightArm, brightness << 1, 0, 0);
					APA102_SetAll(&leftLeg, brightness, brightness, 0);
					APA102_SetAll(&rightLeg, brightness, brightness, 0);
				}
			}
			else if (mode == 0) {
				int i;
				for(i = 0; i < SPECTRUM_COUNT; i++) {
					APA102_SetAll(&(spectrum[i]), 0, 0, 0);
				}
				APA102_SetAll(&leftArm, 0, 0, 0);
				APA102_SetAll(&rightArm, 0, 0, 0);
				APA102_SetAll(&leftLeg, 0, 0, 0);
				APA102_SetAll(&rightLeg, 0, 0, 0);
				
				APA102_Send(&strip);
				Util_WaitMillis(10);
			}
			//for(i = 0; i < 10; i++)
				APA102_Send(&strip);
			lastMode = mode;
		}
		
	}
}

inline void FillSegmentUp(APA102_Segment *segment, uint8_t count, uint8_t r, uint8_t g, uint8_t b) {
	uint8_t i;
	
	for(i = 0; i < count; i++) {
		APA102_SetPixel(segment, i, r, g, b);
	}
	
	for(i = count; i < segment->size/4; i++) {
		APA102_SetPixel(segment, i, 0, 0, 0);
	}
}

inline void FillSegmentDown(APA102_Segment *segment, uint8_t count, uint8_t r, uint8_t g, uint8_t b) {
	uint8_t i, last = segment->size/4 - 1;
	
	for(i = 0; i < count; i++) {
		APA102_SetPixel(segment, last - i, r, g, b);
	}
	
	for(i = count; i < segment->size/4; i++) {
		APA102_SetPixel(segment, last - i, 0, 0, 0);
	}
}

inline uint16_t LogScale(uint16_t val) {
	float temp = (logf(val))*29.f;
	
	return (uint16_t) ((temp > 0) ? temp : 0);
	
}

inline int16_t min(int16_t a, int16_t b) {
	return (a < b) ? a : b;
}

inline int16_t max(int16_t a, int16_t b) {
	return (a > b) ? a : b;
}

void inline Filter(int16_t *val, int16_t newVal, uint16_t strength) {
	int32_t temp = (int32_t)(*val) * strength / 100 + (int32_t)newVal * (100 - strength) / 100;
	
	*val = (int16_t)temp;
}