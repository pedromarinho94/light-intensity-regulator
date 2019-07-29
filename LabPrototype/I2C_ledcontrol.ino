#include <Wire.h>


const uint8_t adress = 0x70;                              //temos como ultimo bit 0 para escrever (1 para ler)



void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);                           //valor comum para isto
  delay(100);                                   //dilay por que sim

  Wire.setClock(100000);
  Wire.beginTransmission(adress);               //começamos por chamar o slave led backpack
  Wire.write(0x21);                             //ligamos o oscilador
  Wire.endTransmission();

  Wire.beginTransmission(adress);               //começamos por chamar o slave led backpack
  Wire.write(0xA0);                             //Row output
  Wire.endTransmission();
  

  Wire.beginTransmission(adress);               //começamos por chamar o slave led backpack
  Wire.write(0xEF);                             //dimming set
  Wire.endTransmission();

  Wire.beginTransmission(adress);
  Wire.write(0x82);                             //escolha da frequencia e Blinking on
  Wire.endTransmission();
  
  Wire.beginTransmission(adress);
  Wire.write(0x00);
  Wire.write(0x00); //o nosso padrão acaba aqui

  for (int i = 0;i<15;i++)
  {
       Wire.write(0x00);
       Wire.write(0xFF);
  }
  
  Wire.endTransmission();

  Wire.beginTransmission(adress);
  Wire.write(0x81);                             //escolha da frequencia e Blinking on
  Wire.endTransmission();
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);
  Serial.print("KEK\n");
  
}
