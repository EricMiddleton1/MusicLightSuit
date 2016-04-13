/*
 * SPI.h
 *
 * Created: 10/14/2015 7:12:15 PM
 *  Author: ericm
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

typedef struct {
	uint8_t *data;
	uint16_t size;
	uint16_t start;
} SPI_RawBuffer;

//False when TX done
#define SPI_TX_DONE			(SPSR & (1 << SPIF))

void SPI_Init();

void SPI_Send(SPI_RawBuffer *raw);
void SPI_SendSewingMachine(SPI_RawBuffer *stitchingData[], uint8_t numberOfElements);
void SPI_SendSewingMachineCirc(SPI_RawBuffer *sticthingData[], uint8_t numberOfElements);

void inline SPI_Write(uint8_t *data);


#endif /* SPI_H_ */