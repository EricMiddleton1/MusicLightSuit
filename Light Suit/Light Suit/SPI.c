/*
 * SPI.c
 *
 * Created: 10/14/2015 7:12:26 PM
 *  Author: ericm
 */ 
#include "SPI.h"

void SPI_Init() {
	//MOSI on PB3, SCK on PB5
	DDRB |= 0b00101100;
	
	SPCR = (1 << MSTR) | (1 << SPE);
	SPSR = 0x01;
}

void inline SPI_Write(uint8_t *data) {
	SPDR = *data;
}

void SPI_Send(SPI_RawBuffer *raw) {
	for(uint16_t spiPtr = 0; spiPtr < raw->size; spiPtr++) {
		SPI_Write(&(raw->data[spiPtr]));
		
		while(!SPI_TX_DONE);
	}
}

void SPI_SendSewingMachine(SPI_RawBuffer *stitchingData[], uint8_t numberOfElements) {
	int i = 0, j = 0;
	SPI_RawBuffer *curBuffer = (stitchingData[0]);
	
	while(1) {
		SPI_Write(&(curBuffer->data[j]));
		j++;
		
		if(j >= curBuffer->size) {
			i++;
			if(i >= numberOfElements)
				break;
			curBuffer = (stitchingData[i]);
			j = 0;
		}
		
		while(!SPI_TX_DONE);
	}
}

void SPI_SendSewingMachineCirc(SPI_RawBuffer *stitchingData[], uint8_t numberOfElements) {
	int i = 0, j = 0;
	SPI_RawBuffer *curBuffer = (stitchingData[0]);
	
	//k = curBuffer->start;
	
	while(1) {
		int k = j + curBuffer->start;
		if(k >= curBuffer->size)
			k -= curBuffer->size;
		
		SPI_Write(&(curBuffer->data[ k ]));
		j++;
		
		if(j >= curBuffer->size) {
			i++;
			
			if(i >= numberOfElements)
				break;
			
			curBuffer = (stitchingData[i]);
			j = 0;
		}
		
		
		while(!SPI_TX_DONE);
	}
}
