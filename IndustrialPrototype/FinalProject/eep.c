/*
 * eep.c
 *
 *  Created on: Dec 19, 2018
 *      Author: cordoes
 */

#include "twi2c.h"




void eepr_test(void)
{
    uint16_t h = eeprom_read_word((const uint16_t*)0x45); /*guardar a hash que está na eeprom*/
    uint8_t b;
    uint16_t erros = 0;
    register uint16_t e;


    for (e=0; e<1024;e++)
    {
        eeprom_write_byte((uint8_t*)e, 0x00U);
    }
    for(e=0; e<1024;e++)
    {
        b=eeprom_read_byte((const uint8_t*)e);
        if (b==0)
        {
            eeprom_write_byte((uint8_t*)e,0xFFU);
        }
        else
        {
            erros+=1;
            eeprom_write_byte((uint8_t*)e,0xFFU);
        }
    }

    for(e=0; e<1024;e++)
    {
        b=eeprom_read_byte((const uint8_t*)e);
        if (b==255)
        {
            eeprom_write_byte((uint8_t*)e,0x00U);
        }
        else
        {
            erros+=1;
            eeprom_write_byte((uint8_t*)e,0x00U);
        }
    }
    for (e=0; e<1024;e++)
    {
        b=eeprom_read_byte((const uint8_t*)e);
        if(b!=0)
        {
            erros+=1;
        }
        else
        {
        	;
        }
    }

    eeprom_write_word((uint16_t*)0x45,h);

    if (erros==0)
    {
        montagem(0x87U,0x81U);
    }
    else
    {
    	montagem(0x87U,0x83U);
	    i2c_start(LED);
	    i2c_write(0xE0U);
	    i2c_stop();
    	while(1)
    	{
    		_delay_ms(300); /*failed test*/
    	}
    }
}

