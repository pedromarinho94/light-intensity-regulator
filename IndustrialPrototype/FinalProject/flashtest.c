/*
 * flashtest.c
 *
 *  Created on: Dec 19, 2018
 *      Author: cordoes
 */



#include "flashtest.h"
#include <avr/pgmspace.h>
#include "twi2c.h"


#define N_BYTES_FLASH (2430U)

uint16_t flash_test_calculate_xor(void);


void flash_test(void) {
	uint16_t hash = flash_test_calculate_xor();

    uint16_t true_hash = eeprom_read_word ((const uint16_t*)0x45);
    eeprom_write_word((uint16_t*)0x50, hash);

    if (hash==true_hash)
    {
    	montagem(0xFFU,0x81U);
    }
    else
    {
		montagem(0xFFU,0x83U);
	    i2c_start(LED);
	    i2c_write(0xE0U);
	    i2c_stop();
    	while(1)
    	{
    		_delay_ms(300); /*failed test*/
    	}
    }

}

uint16_t flash_test_calculate_xor(void) {
	uint16_t hash = 0;

	for (uint16_t k = 0; k < N_BYTES_FLASH - 1; k += 2) {
		uint16_t value = pgm_read_word_near(k);
	    hash ^= value;
	}

	return hash;
}
