/*
 * main.c
 *
 *  Created on: 10/12/2018
 *      Author: Marinho
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define CS 2
#define MOSI 3
#define MISO 4
#define SCK 5

int main (void) {

	DDRB |= (1<<CS)|(1<<MOSI)|(1<<SCK);    // SCK, MOSI and SS as outputs
	DDRB &= ~(1<<MISO);                    // MISO as input
	PORTB |= (1<<CS); 				       //Set slave high

	DDRB |= (1<<0);

	SPCR |= (1<<MSTR);              	   // Set as Master

	SPCR |= (1<<SPR0);   	  	   		   // divided clock by 16 (1MHZ)
	SPCR &= ~(1<<SPR1);					   // divided clock by 16 (1MHZ)

	SPCR &= ~(1<<CPOL);   	  	   		   // MODE 0
	SPCR &= ~(1<<CPHA);					   // MODE 0

	SPCR &= ~(1<<DORD);				 	   // MSB first
	SPCR |= (1<<SPE);              		   // Enable SPI

	uint8_t dataIn[2];
	uint8_t i;
	uint16_t lumiere;

 while(1) {

	PORTB &= ~(1<<CS);					//set CS low - start transaction

	for (i=0;i<2;i=i+1)
	{
		SPDR = 0x00;						//send trash
		while (!(SPSR & (1<<SPIF)));		//wait for last bit to be sent
		dataIn[i] = SPDR;						//save data
	}

	PORTB |= (1<<CS);  					// set CS high - deactivate

	lumiere=(dataIn[0]<<4)|(dataIn[1]>>4); // Reconstitution of the 8-bit light variable

	if(lumiere>100)
		PORTB &= ~(1<<0);
	else
		PORTB |= (1<<0);

	_delay_ms(1000);
 }

}


/* the read/write functions are incomplete, ie. a slave read, but no slave write, visa versa, a master write, but no master read. */
