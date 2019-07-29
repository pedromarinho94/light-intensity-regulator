#include <Wire.h>
#include <SPI.h> // call library

#define CS 10 // Assignment of the CS pin

int i;
unsigned char recu[2]; // Storage of module data
unsigned char lumiere;


const uint8_t adress = 0x70;                              //temos como ultimo bit 0 para escrever (1 para ler)
uint8_t brilho = 0xE0;


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);                           //valor comum para isto
  SPI.begin(); // initialization of SPI port
  SPI.setDataMode(SPI_MODE0); // configuration of SPI communication in mode 0
  SPI.setClockDivider(SPI_CLOCK_DIV2); // configuration of clock at 1MHz
  pinMode(CS, OUTPUT);
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
  Wire.write(0xFF); //o nosso padrão acaba aqui
/*
  for (int i = 0;i<15;i++)
  {
       Wire.write(0x00);
       Wire.write(0xFF);
  }
*/
  
  Wire.endTransmission();

  Wire.beginTransmission(adress);
  Wire.write(0x81);                             //escolha da frequencia e Blinking on
  Wire.endTransmission();
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
    delay(500);
    brilho=0xE0;
    digitalWrite(CS, LOW); // activation of CS line
    for (i=0;i<2;i=i+1)
    {
    recu[i] = SPI.transfer(1); // Acquisition of the 2 bytes of data
    }
    digitalWrite(CS, HIGH); // deactivation of CS line

    for (i=0;i<2;i=i+1) // Send in serial monitor
    {
    Serial.print("i");
    Serial.print(i);
    Serial.print("=");
    Serial.println(recu[i],BIN);
    }


    lumiere=(recu[0]<<4)|(recu[1]>>4);    
    Serial.print("Lumiere=");
    Serial.println(lumiere);
    lumiere=(256-lumiere)/16;
    //lumiere=(lumiere)/16;
    brilho=brilho + lumiere;

    
    Wire.beginTransmission(adress);               //começamos por chamar o slave led backpack
    Wire.write(brilho);                             //dimming set
    Wire.endTransmission();


  

}
