#include "twi2c.h"
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "flashtest.h"
#include "sram_test.h"



void loading(void)
{
    uint8_t onde = 0x00;
    uint8_t qual = 0x00;
    uint8_t este = 0x01;
    int erro = 0;
    int a;

    montagem(0x00);


    for (int i=0; i<64; i++)
    {
      if (i%8==7)
      {
         este|=1<<i%8;
         erro+=ler(onde,qual);
         escrever(onde,este);
         onde+=2;
         este=0x01;
         qual=0x00;
         //Serial.print(erro);
         //Serial.print("\n");

         _delay_ms(100);
      }
      else
      {
        este|=1<<i%8;
        erro+=ler(onde,qual);
        escrever(onde,este);
        qual|=1<<i%8;
        _delay_ms(100);
      }

    }
    onde=0x0E;
    este=0xFF;
    qual=0xFF;

   for (int i=0; i<64; i++)
   {
     if (i%8==7)
     {
           a=7-i%8;
           este&=~(1<<a);
           erro+=ler(onde,qual);
           escrever(onde,este);
           onde-=2;
           este=0xFF;
           qual=0xFF;
           _delay_ms(100);
      }
     else
     {
          a=7-i%8;
          este&=~(1<<a);
          erro+=ler(onde,qual);
          escrever(onde,este);
          qual&=~(1<<a);
          _delay_ms(100);
     }
    }
    onde=0x00;
    qual=0x00;
    for (int i=0; i<8;i++);
    {
        erro+=ler(onde,qual);
        onde+=2;
    }
    if (erro==0){
        montagem(0x81);
    }
    _delay_ms(2000);
    flash_test();
    _delay_ms(2000);
    sram_test();
    _delay_ms(2000);


}

int ler (uint8_t morada, uint8_t imi)
{
  uint8_t ca;
  //Wire.beginTransmission(adress);
  i2c_start(LED);
  //Wire.write(morada);
  i2c_write(morada);
  //Wire.endTransmission();
  i2c_rep_start(LED+1);
  //Wire.requestFrom(adress,1);
  //ca=Wire.read();
  ca=i2c_readNak();
  i2c_stop();
  if(ca==imi)
  {
    return 0;
  }
  else
  {
    return 1;
  }

}

void escrever (uint8_t morada, uint8_t pad)
{
  //Wire.beginTransmission(adress);
  i2c_start(LED);
  //Wire.write(morada);
  i2c_write(morada);
  //Wire.write(pad);
  i2c_write(pad);
  i2c_stop();
}
