#include <twi2c.h>
#include "flashtest.h"
#include "sram_test.h"



void loading(void)
{
    static uint8_t onde = 0x00U;
    static uint8_t qual = 0x00U;
    static uint8_t este = 0x01U;
    static uint8_t erro = 0U;
    static uint8_t a;
    register uint16_t e;


    montagem(0x00U);


    for ( e=0; e<64; e++)
    {
      if (e%8==7)
      {
         este|=(uint8_t)(1<<e%8);
         erro+=ler(onde,qual);
         escrever(onde,este);
         onde+=2U;
         este=0x01U;
         qual=0x00U;
         _delay_ms(100);
      }
      else
      {
        este|=(uint8_t)(1<<e%8);
        erro+=ler(onde,qual);
        escrever(onde,este);
        qual|=(uint8_t)1<<e%8;
        _delay_ms(100);
      }

    }
    onde=0x0EU;
    este=0xFFU;
    qual=0xFFU;

   for (e=0; e<64; e++)
   {
     if (e%8==7)
     {
           a=(uint8_t)(7-e%8);
           este&=~(1<<a);
           erro+=ler(onde,qual);
           escrever(onde,este);
           onde-=2;
           este=0xFFU;
           qual=0xFFU;
           _delay_ms(100);
      }
     else
     {
          a=(uint8_t)(7-e%8);
          este&=~(1<<a);
          erro+=ler(onde,qual);
          escrever(onde,este);
          qual&=~(1<<a);
          _delay_ms(100);
     }
    }
    onde=0x00U;
    qual=0x00U;
    for ( e=0; e<8;e++);
    {
        erro+=ler(onde,qual);
        onde+=2U;
    }
    if (0==erro){
        montagem(0x81U);
    }
    else
    {
        ;
    }
    _delay_ms(2000);
    flash_test();
    _delay_ms(2000);
    sram_test();
    _delay_ms(2000);
    eepr_test();

}

uint8_t ler (uint8_t morada, uint8_t imi)
{
  static uint8_t ca;

  i2c_start(LED);

  i2c_write(morada);

  i2c_rep_start(LED+1U);

  ca=i2c_readNak();
  i2c_stop();
  if(ca==imi)
  {
    return 0U;
  }
  else
  {
    return 1U;
  }

}

void escrever (uint8_t morada, uint8_t pad)
{

  i2c_start(LED);

  i2c_write(morada);

  i2c_write(pad);
  i2c_stop();
}
