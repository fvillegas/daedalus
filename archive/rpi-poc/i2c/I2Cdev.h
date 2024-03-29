/**
 * clone from https://github.com/rpicopter/MotionSensorExample/tree/master/libs/I2Cdev
 * rename function and variables to make more sense for me
 */

// I2Cdev library collection - Main I2C device class header file
// Abstracts bit and byte I2C R/W functions into a convenient class
// 6/9/2012 by Jeff Rowberg <jeff@rowberg.net>
//
// Changelog:
//     2012-06-09 - fix major issue with reading > 32 bytes at a time with Arduino Wire
//                - add compiler warnings when using outdated or IDE or limited I2Cdev implementation
//     2011-11-01 - fix write*Bits mask calculation (thanks sasquatch @ Arduino forums)
//     2011-10-03 - added automatic Arduino version detection for ease of use
//     2011-10-02 - added Gene Knight's NBWire TwoWire class implementation with small modifications
//     2011-08-31 - added support for Arduino 1.0 Wire library (methods are different from 0.x)
//     2011-08-03 - added optional timeout parameter to read* methods to easily change from default
//     2011-08-02 - added support for 16-bit registers
//                - fixed incorrect Doxygen comments on some methods
//                - added timeout value for read operations (thanks mem @ Arduino forums)
//     2011-07-30 - changed read/write function structures to return success or byte counts
//                - made all methods static for multi-device memory savings
//     2011-07-28 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#define I2C_OK 0
#define I2C_ERR -1

int8_t read_bit_word(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint16_t *data);
int8_t read_bits(uint8_t dev_addr, uint8_t reg_addr, uint8_t bitStart, uint8_t length, uint8_t *data);
int8_t read_bits_word(uint8_t dev_addr, uint8_t reg_addr, uint8_t bitStart, uint8_t length, uint16_t *data);
int8_t read_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data);
int8_t read_word(uint8_t dev_addr, uint8_t reg_addr, uint16_t *data);
int8_t read_bytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t *data);
int8_t read_words(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint16_t *data);

int write_bit(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint8_t data);
int write_bit_word(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint16_t data);
int write_bits(uint8_t dev_addr, uint8_t reg_addr, uint8_t bitStart, uint8_t length, uint8_t data);
int write_bits_word(uint8_t dev_addr, uint8_t reg_addr, uint8_t bitStart, uint8_t length, uint16_t data);
int write_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
int write_word(uint8_t dev_addr, uint8_t reg_addr, uint16_t data);
int write_bytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t *data);
int write_words(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint16_t *data);

#endif /* _I2CDEV_H_ */
