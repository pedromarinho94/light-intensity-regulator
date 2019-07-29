
#include "flashtest.h"
#include <avr/pgmspace.h>
#include <twi2c.h>


#define N_BYTES_FLASH (1956U)

uint16_t flash_test_calculate_xor(void);


void flash_test(void) {
    static uint8_t rwrong;
	uint16_t hash = flash_test_calculate_xor();

    uint16_t true_hash = eeprom_read_word ((const uint16_t*)0x45);
    eeprom_write_word((uint16_t*)0x50, hash);

    if (hash==true_hash)
    {
        rwrong = 0x87U;
    }
    else
    {
        rwrong = 0x00U;
    }
    montagem(rwrong);

}

uint16_t flash_test_calculate_xor(void) {
	uint16_t hash = 0;

	for (uint16_t k = 0; k < N_BYTES_FLASH - 1; k += 2) {
		uint16_t value = pgm_read_word_near(k);
	    hash ^= value;
	}

	return hash;
}
