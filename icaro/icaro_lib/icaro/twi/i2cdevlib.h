#ifndef __I2C_DEV_LIB_
#define __I2C_DEV_LIB_

#include <inttypes.h>

#define I2CDEV_DEFAULT_READ_TIMEOUT     1000

int8_t i2c_read_bit(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint8_t *data, uint16_t timeout);
int8_t i2c_read_bit_word(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint16_t *data, uint16_t timeout);
int8_t i2c_read_bits(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_start, uint8_t length, uint8_t *data, uint16_t timeout);
int8_t i2c_read_bits_word(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_start, uint8_t length, uint16_t *data, uint16_t timeout);
int8_t i2c_read_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t timeout);
int8_t i2c_read_word(uint8_t dev_addr, uint8_t reg_addr, uint16_t *data, uint16_t timeout);
int8_t i2c_read_bytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t *data, uint16_t timeout);
int8_t i2c_read_words(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint16_t *data, uint16_t timeout);

uint8_t i2c_write_bit(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint8_t data);
uint8_t i2c_write_bit_word(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint16_t data);
uint8_t i2c_write_bits(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_start, uint8_t length, uint8_t data);
uint8_t i2c_write_bits_word(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_start, uint8_t length, uint16_t data);
uint8_t i2c_write_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
uint8_t i2c_write_word(uint8_t dev_addr, uint8_t reg_addr, uint16_t data);
uint8_t i2c_write_bytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t *data);
uint8_t i2c_write_words(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint16_t *data);

#endif