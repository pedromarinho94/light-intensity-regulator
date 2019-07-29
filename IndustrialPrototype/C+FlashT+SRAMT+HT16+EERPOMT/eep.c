#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <twi2c.h>




void eepr_test(void)
{
    uint16_t h = eeprom_read_word((uint16_t*)0x45); //guardar a hash que está na eeprom
    uint8_t a;
    uint8_t erro = 0;
    for (int i=0; i<1024;i++)
    {
        eeprom_write_byte((uint8_t*)i, 0x00);
    }
    for(int i=0; i<1024;i++)
    {
        a=eeprom_read_byte((uint8_t*)i);
        if (a==0)
        {
            eeprom_write_byte((uint8_t*)i,0xFF);
        }
        else
        {
            erro+=1;
            eeprom_write_byte((uint8_t*)i,0xFF);
        }
    }
    //erro+=1;
    //eeprom_write_byte((uint8_t*)0,erro);
    for(int i=0; i<1024;i++)
    {
        a=eeprom_read_byte((uint8_t*)i);
        if (a==255)
        {
            eeprom_write_byte((uint8_t*)i,0x00);
        }
        else
        {
            erro+=1;
            eeprom_write_byte((uint8_t*)i,0x00);
        }
    }
    for (int i=0; i<1024;i++)
    {
        a=eeprom_read_byte((uint8_t*)i);
        if(a!=0)
        {
            erro+=1;
        }
    }
    //faz a merda que quiseres com os erros.
    eeprom_write_word((uint16_t*)0x45,h);

    if (0==erro)
    {
        montagem(0xFF);
    }
}
