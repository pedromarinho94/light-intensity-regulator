
#include "flashtest.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "twi2c.h"


#define N_BYTES_FLASH (762U)
//762 numero que está no merdas
//32256 numero do arduino
//
//uint16_t EEMEM eptr_eeprom_code_hash = 0x45;
uint16_t flash_test_calculate_xor(void);

uint8_t rwrong;

void flash_test(void) {
	uint16_t hash = flash_test_calculate_xor();
    //uint16_t hash = 45814;
	//uint16_t true_hash = eeprom_read_word(&eptr_eeprom_code_hash);
    uint16_t true_hash = eeprom_read_word ((const uint16_t*)0x45);
    eeprom_write_word((uint16_t*)0x50, hash);
    //uint16_t true_hash = 45814;
	/*debug_send_test_result((hash == true_hash) ? (0) : (255));*/
    if (hash==true_hash)
    {
        rwrong = 0xFF;
    }
    else
    {
        rwrong = 0x00;
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
