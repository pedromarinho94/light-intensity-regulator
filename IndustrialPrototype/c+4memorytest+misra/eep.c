#include <twi2c.h>




void eepr_test(void)
{
    uint16_t h = eeprom_read_word((const uint16_t*)0x45); /*guardar a hash que está na eeprom*/
    static uint8_t b;
    static uint8_t erros = 0U;
    register uint16_t e = 0;


    for (e=0; e<1024;e++)
    {
        eeprom_write_byte((uint8_t*)e, 0x00U);
    }
    for(e=0; e<1024;e++)
    {
        b=eeprom_read_byte((const uint8_t*)e);
        if (0==b)
        {
            eeprom_write_byte((uint8_t*)e,0xFFU);
        }
        else
        {
            erros+=1U;
            eeprom_write_byte((uint8_t*)e,0xFFU);
        }
    }

    for(e=0; e<1024;e++)
    {
        b=eeprom_read_byte((const uint8_t*)e);
        if (255==b)
        {
            eeprom_write_byte((uint8_t*)e,0x00U);
        }
        else
        {
            erros+=1U;
            eeprom_write_byte((uint8_t*)e,0x00U);
        }
    }
    for (e=0; e<1024;e++)
    {
        b=eeprom_read_byte((const uint8_t*)e);
        if(b!=0)
        {
            erros+=1U;
        }
        else
        {
            ;
        }
    }

    eeprom_write_word((uint16_t*)0x45,h);

    if (0==erros)
    {
        montagem(0xFFU);
    }
    else
    {
        ;
    }
}
