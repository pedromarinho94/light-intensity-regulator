#ifndef _I2CMASTER_H
#define _I2CMASTER_H
#include <inttypes.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
/*#include <avr/io.h>*/

#define I2C_READ 1U
#define I2C_WRITE 0U
#define LED 0xE0U

extern void i2c_init(void);

extern void i2c_stop(void);

extern unsigned char i2c_start(unsigned char address);

extern unsigned char i2c_rep_start(unsigned char address);

extern void i2c_start_wait(unsigned char addr);

extern unsigned char i2c_write(unsigned char data);

extern unsigned char i2c_readAck(void);

extern unsigned char i2c_readNak(void);

extern unsigned char i2c_read(unsigned char ack);
#define i2c_read(ack) (ack) ? i2c_readAck() : i2c_readNak();

extern void montagem (unsigned char padrao);

extern void flash_test(void);

extern uint8_t ler (uint8_t morada, uint8_t imi);

extern void escrever (uint8_t morada, uint8_t pad);

extern void loading(void);

extern void eepr_test(void);
#endif
