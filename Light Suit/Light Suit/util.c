#include "util.h"

volatile unsigned long ms;
volatile unsigned char adcChannel;
volatile unsigned int adc[5], old[5], derivative[5], offset[5];
volatile uint32_t scale[5];

uint8_t ticks = 0;

void Util_Init() {
	cli();
	
	//Init timer0 with /64 prescaler
	//And OCR0A to fire every 1ms
	TCCR0A = 0x00;
	TCCR0B = 0x03;
	OCR0A = 250;
	TIMSK0 = 1 << OCIE0A;
	
	//Init ADC to run everytime OCR0A fires
	//starting with ADC channel 0
	ADMUX = 0b01000111;
	ADCSRA = 0b10101111;
	ADCSRB = 0b00000011;
	DIDR0 = 0b00111111;
	
	//Init port D pins to clear the peak detector
	DDRD |= 0b01111100;
	PORTD &= ~(0b01111100);
	
	//Init calibrate button on PB0
	DDRB &= ~(0x01);
	PORTB |= 0x01;
	
	//Initialize offset
	offset[0] = 0;
	offset[1] = 0;
	offset[2] = 0;
	offset[3] = 0;
	offset[4] = 0;
	
	scale[0] = 1 << 4;
	scale[1] = 1 << 4;
	scale[2] = 1 << 4;
	scale[3] = 1 << 4;
	scale[4] = 1 << 4;
	
	sei();
	
	ms = 0;
	adcChannel = 0;
}

unsigned long Util_GetMillis() {
	unsigned long millis;
	
	cli();
	millis = ms;
	sei();
	
	return millis;
}

void Util_WaitMillis(unsigned int millis) {
	unsigned long endTime = millis + Util_GetMillis();
	
	while(Util_GetMillis() < endTime);
}

void inline Util_Filter(uint16_t *val, uint16_t newVal, uint16_t strength) {
	*val = (*val) * strength / 100 + newVal * (100 - strength) / 100;
}

void inline Util_FilterSigned(int16_t *val, int16_t newVal, uint16_t strength) {
	*val = (*val) * strength / 100 + newVal * (100 - strength) / 100;
}

ISR(TIMER0_COMPA_vect) {
	TCNT0 = 0;
	ms++;
}

ISR(TIMER0_COMPB_vect) {
	//Release the peak detector
	PORTD &= ~(0b01111100);
	
	//Remove the interrupt
	TIMSK0 &= ~(1 << OCIE0B);
}

ISR(ADC_vect) {
	ticks++;
	
	if(ticks == 5) {
			ticks = 0;
		//record the adc value
		//adc[adcChannel] = (ADCL) | (ADCH << 8);
		uint16_t newValue = (ADCL) | (ADCH << 8), calibrated;
		
		//check the calibrate button
		if(~PINB & 0x01) {
			Util_Filter(&(offset[adcChannel]), newValue, 0.1);
			
			scale[adcChannel] = ((uint32_t)1023 << 4) / (1023 - offset[adcChannel]);
		}
		
		if(offset[adcChannel] >= newValue)
			newValue = 0;
		else {
			newValue -= offset[adcChannel];
			newValue = ((uint32_t)newValue * scale[adcChannel]) >> 4;
		}
	
		Util_Filter( &(adc[adcChannel]), newValue, 20);
		Util_Filter( &(derivative[adcChannel]), newValue - old[adcChannel], 0);
	
		old[adcChannel] = newValue;
		
		//clear the peak detector
		PORTD |= 1 << (adcChannel + 2);
	
		//set OCR0B to release the peak detector
		OCR0B = TCNT0 + 4;	//16uS
		TIMSK0 |= 1 << OCIE0B;
		
		adcChannel++;
	
		if(adcChannel >= 5)
			adcChannel = 0;
	
		//Change the ADC channel for the next conversion
		ADMUX = (ADMUX & 0b11111000) | adcChannel;
	}
	
}