#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include <compat/twi.h>
#include <twi2c.h>
#include "sram_test.h"

#ifndef F_CPU
#define F_CPU 1600000UL
#endif
#define SCL_CLOCK 40000L
#define LED 0xE0
#define CS 2
#define MOSI 3
#define MISO 4
#define SCK 5



int main(void)
{
    uint8_t dataIn[2];
    uint8_t i;
    uint8_t lumiere;
    uint8_t brilho=0xEF;
    uint8_t lumiere2;
    uint8_t padrao =0xF0;
    flash_test();
    _delay_ms(2000);
    montagem (padrao);
    _delay_ms(2000);
    sram_test();
    _delay_ms(2000);
    padrao=0xF0;
    montagem (padrao);
    _delay_ms(4000);
    padrao=0xFF;
    montagem(padrao);

    /*SETUP DE SPI*/
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

    /*END OF SPI SETUP*/

    /*Environment Light Intensity Regulator*/


     while(1)
     {
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

        lumiere2=(255-lumiere)/16;
        brilho=0xE0;
        brilho=brilho+lumiere2;
        i2c_start(LED);
        i2c_write(brilho);
        i2c_stop();
        _delay_ms(400);

     }




}


void i2c_init(void)
{
    TWSR = 0;
    TWBR =((F_CPU/SCL_CLOCK)-16)/2;
}

unsigned char i2c_start(unsigned char address)
{
    uint8_t twst;
    /*We send the start*/
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    /*We wait for the transmission to complete*/
    while(!(TWCR & (1<<TWINT)))
    {
        ;
    }
    /*We want to check TWSR while masking the prescaler*/
    twst = TW_STATUS & 0xF8;
    if ( (twst != TW_START ) && ( twst != TW_REP_START))
    {
        return 1;
    }
    /*We send the address*/
    TWDR = address;
    TWCR = (1<<TWINT) | (1<<TWEN);

    while (!(TWCR & (1<<TWINT)))
    {
        ;
    }

    twst = TW_STATUS & 0xF8;
    if ((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK))
    {
            return 1;
    }

    return 0;
}

void i2c_start_wait(unsigned char address)
{
    uint8_t twst;

    while ( 1 )
    {
        TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

        while(!(TWCR & (1<<TWINT)))
        {
                ;
        }

        twst = TW_STATUS & 0xF8;

        if ( (twst != TW_START) && (twst != TW_REP_START))
        {
             continue;
        }

        TWDR = address;
        TWCR = (1<<TWINT) | (1<<TWEN);

        while(!(TWCR & (1<<TWINT)));

        twst = TW_STATUS & 0xF8;
        if ( (twst == TW_MT_SLA_NACK )||(twst == TW_MR_DATA_NACK) )
        {
            /* device busy, send stop condition to terminate write operation */
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

            // wait until stop condition is executed and bus released
            while(TWCR & (1<<TWSTO));

            continue;
        }
        //if( twst != TW_MT_SLA_ACK) return 1;
        break;
    }

}

unsigned char i2c_rep_start(unsigned char address)
{
    return i2c_start( address );
}

void i2c_stop(void)
{
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

    while (TWCR & (1<<TWSTO));
}

unsigned char i2c_write(unsigned char data)
{
    uint8_t twst;
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);

    while (!(TWCR & (1<<TWINT)))
    {
        ;
    }
    twst = TW_STATUS & 0xF8;
    if ( twst != TW_MT_DATA_ACK)
    {
        return 1;
    }

    return 0;

}

unsigned char i2c_readAck(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while(!(TWCR & (1<<TWINT)))
    {
        ;
    }
    return TWDR;
}

unsigned char i2c_readNak(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)))
    {
        ;
    }
    return TWDR;
}

void montagem(unsigned char padrao)
{
    i2c_init();

    i2c_start(LED);
    i2c_write(0x21);
    i2c_stop();

    i2c_start(LED);
    i2c_write(0xA0);
    i2c_stop();

    i2c_start(LED);
    i2c_write(0xEF);
    i2c_stop();

    i2c_start(LED);
    i2c_write(0x82);
    i2c_stop();

    i2c_start(LED);
    i2c_write(0x00);
    i2c_write(padrao);
    for (int i = 0; i<15 ; i++)
    {
        i2c_write(0x00);
        i2c_write(padrao);
    }
    i2c_stop();

    i2c_start(LED);
    i2c_write(0x81);
    i2c_stop();
}
