// I2Cdev library collection - Main I2C device class
// Abstracts bit and byte I2C R/W functions into a convenient class
// 2013-06-05 by Jeff Rowberg <jeff@rowberg.net>
//
// Changelog:
//      2013-05-06 - add Francesco Ferrara's Fastwire v0.24 implementation with small modifications
//      2013-05-05 - fix issue with writing bit values to words (Sasquatch/Farzanegan)
//      2012-06-09 - fix major issue with reading > 32 bytes at a time with Arduino Wire
//                 - add compiler warnings when using outdated or IDE or limited I2Cdev implementation
//      2011-11-01 - fix write*Bits mask calculation (thanks sasquatch @ Arduino forums)
//      2011-10-03 - added automatic Arduino version detection for ease of use
//      2011-10-02 - added Gene Knight's NBWire TwoWire class implementation with small modifications
//      2011-08-31 - added support for Arduino 1.0 Wire library (methods are different from 0.x)
//      2011-08-03 - added optional timeout parameter to read* methods to easily change from default
//      2011-08-02 - added support for 16-bit registers
//                 - fixed incorrect Doxygen comments on some methods
//                 - added timeout value for read operations (thanks mem @ Arduino forums)
//      2011-07-30 - changed read/write function structures to return success or byte counts
//                 - made all methods static for multi-device memory savings
//      2011-07-28 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2013 Jeff Rowberg

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

#include <stdbool.h>

#include "i2cdevlib.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#ifndef BUFFER_LENGTH
// band-aid fix for platforms without Wire-defined BUFFER_LENGTH (removed from some official implementations)
#define BUFFER_LENGTH 32
#endif

uint8_t BUFFER[32] = {0};

/** Read a single bit from an 8-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr Register reg_addr to read from
* @param bit_num Bit position to read (0-7)
* @param data Container for single bit value
* @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
* @return Status of read operation (true = success)
*/
int8_t i2c_read_bit(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint8_t *data, uint16_t timeout) {
    uint8_t b;
    uint8_t count = i2c_read_byte(dev_addr, reg_addr, &b, timeout);
    *data = b & (1 << bit_num);
    return count;
}

/** Read a single bit from a 16-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr Register reg_addr to read from
* @param bit_num Bit position to read (0-15)
* @param data Container for single bit value
* @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
* @return Status of read operation (true = success)
*/
int8_t i2c_read_bit_word(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint16_t *data, uint16_t timeout) {
    uint16_t b;
    uint8_t count = i2c_read_word(dev_addr, reg_addr, &b, timeout);
    *data = b & (1 << bit_num);
    return count;
}

/** Read multiple bits from an 8-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr Register reg_addr to read from
* @param bit_start First bit position to read (0-7)
* @param length Number of bits to read (not more than 8)
* @param data Container for right-aligned value (i.e. '101' read from any bit_start position will equal 0x05)
* @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
* @return Status of read operation (true = success)
*/
int8_t i2c_read_bits(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_start, uint8_t length, uint8_t *data, uint16_t timeout) {
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bit_start=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t count, b;
    if ((count = i2c_read_byte(dev_addr, reg_addr, &b, timeout)) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bit_start - length + 1);
        b &= mask;
        b >>= (bit_start - length + 1);
        *data = b;
    }
    return count;
}

/** Read multiple bits from a 16-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr Register reg_addr to read from
* @param bit_start First bit position to read (0-15)
* @param length Number of bits to read (not more than 16)
* @param data Container for right-aligned value (i.e. '101' read from any bit_start position will equal 0x05)
* @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
* @return Status of read operation (1 = success, 0 = failure, -1 = timeout)
*/
int8_t i2c_read_bits_word(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_start, uint8_t length, uint16_t *data, uint16_t timeout) {
    // 1101011001101001 read byte
    // fedcba9876543210 bit numbers
    //    xxx           args: bit_start=12, length=3
    //    010           masked
    //           -> 010 shifted
    uint8_t count;
    uint16_t w;
    if ((count = i2c_read_word(dev_addr, reg_addr, &w, timeout)) != 0) {
        uint16_t mask = ((1 << length) - 1) << (bit_start - length + 1);
        w &= mask;
        w >>= (bit_start - length + 1);
        *data = w;
    }
    return count;
}

/** Read single byte from an 8-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr Register reg_addr to read from
* @param data Container for byte value read from device
* @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
* @return Status of read operation (true = success)
*/
int8_t i2c_read_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t timeout) {
    return i2c_read_bytes(dev_addr, reg_addr, 1, data, timeout);
}

/** Read single word from a 16-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr Register reg_addr to read from
* @param data Container for word value read from device
* @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
* @return Status of read operation (true = success)
*/
int8_t i2c_read_word(uint8_t dev_addr, uint8_t reg_addr, uint16_t *data, uint16_t timeout) {
    return i2c_read_words(dev_addr, reg_addr, 1, data, timeout);
}

/** Read multiple bytes from an 8-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr First register reg_addr to read from
* @param length Number of bytes to read
* @param data Buffer to store read data in
* @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
* @return Number of bytes read (-1 indicates failure)
*/
int8_t i2c_read_bytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t *data, uint16_t timeout) {
    int8_t count = 0;
    uint32_t t1 = millis();
    BUFFER[0] = {reg_addr};
    for (uint8_t k = 0; k < length; k += min((int)length, BUFFER_LENGTH)) {
        twi_write_to(dev_addr, BUFFER, 1, true, false);
        count = twi_read_from(dev_addr, data, (uint8_t)min(length - k, BUFFER_LENGTH), false);
    }
    BUFFER = {0};
    return count;
}

/** Read multiple words from a 16-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr First register reg_addr to read from
* @param length Number of words to read
* @param data Buffer to store read data in
* @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
* @return Number of words read (-1 indicates failure)
*/
int8_t i2c_read_words(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint16_t *data, uint16_t timeout) {
    int8_t count = 0;
    uint32_t t1 = millis();
    
    uint8_t BUFFER[0] = {reg_addr};
    for (uint8_t k = 0; k < length * 2; k += min(length * 2, BUFFER_LENGTH)) {
        twi_write_to(dev_addr, BUFFER, 1, true, false);
        twi_read_from(dev_addr, BUFFER, (uint8_t)(length * 2), true);   
        bool msb = true; // starts with MSB, then LSB
        for (; count < length && (timeout == 0 || millis() - t1 < timeout);) {
            if (msb) {
                // first byte is bits 15-8 (MSb=15)
                data[count] = BUFFER[count++] << 8;
            } else {
                // second byte is bits 7-0 (LSb=0)
                data[count] |= BUFFER[count++];
            }
            msb = !msb;
        }
    }
    return count;
}

/** write a single bit in an 8-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr Register reg_addr to write to
* @param bit_num Bit position to write (0-7)
* @param value New bit value to write
* @return Status of operation (true = success)
*/
uint8_t i2c_write_bit(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint8_t data) {
    uint8_t b;
    i2c_read_byte(dev_addr, reg_addr, &b);
    b = (data != 0) ? (b | (1 << bit_num)) : (b & ~(1 << bit_num));
    return i2c_write_byte(dev_addr, reg_addr, b);
}

/** write a single bit in a 16-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr Register reg_addr to write to
* @param bit_num Bit position to write (0-15)
* @param value New bit value to write
* @return Status of operation (true = success)
*/
uint8_t i2c_write_bit_word(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_num, uint16_t data) {
    uint16_t w;
    i2c_read_word(dev_addr, reg_addr, &w);
    w = (data != 0) ? (w | (1 << bit_num)) : (w & ~(1 << bit_num));
    return i2c_write_word(dev_addr, reg_addr, w);
}

/** Write multiple bits in an 8-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr Register reg_addr to write to
* @param bit_start First bit position to write (0-7)
* @param length Number of bits to write (not more than 8)
* @param data Right-aligned value to write
* @return Status of operation (true = success)
*/
uint8_t i2c_write_bits(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_start, uint8_t length, uint8_t data) {
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bit_start=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t b;
    if (i2c_read_byte(dev_addr, reg_addr, &b) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bit_start - length + 1);
        data <<= (bit_start - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return i2c_write_byte(dev_addr, reg_addr, b);
    } else {
        return false;
    }
}

/** Write multiple bits in a 16-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr Register reg_addr to write to
* @param bit_start First bit position to write (0-15)
* @param length Number of bits to write (not more than 16)
* @param data Right-aligned value to write
* @return Status of operation (true = success)
*/
uint8_t i2c_write_bits_word(uint8_t dev_addr, uint8_t reg_addr, uint8_t bit_start, uint8_t length, uint16_t data) {
    //              010 value to write
    // fedcba9876543210 bit numbers
    //    xxx           args: bit_start=12, length=3
    // 0001110000000000 mask word
    // 1010111110010110 original value (sample)
    // 1010001110010110 original & ~mask
    // 1010101110010110 masked | value
    uint16_t w;
    if (i2c_read_word(dev_addr, reg_addr, &w) != 0) {
        uint16_t mask = ((1 << length) - 1) << (bit_start - length + 1);
        data <<= (bit_start - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        w &= ~(mask); // zero all important bits in existing word
        w |= data; // combine data with existing word
        return i2c_write_word(dev_addr, reg_addr, w);
    } else {
        return false;
    }
}

/** Write single byte to an 8-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr Register address to write to
* @param data New byte value to write
* @return Status of operation (true = success)
*/
uint8_t i2c_write_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
    return i2c_write_bytes(dev_addr, reg_addr, 1, &data);
}

/** Write single word to a 16-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr Register address to write to
* @param data New word value to write
* @return Status of operation (true = success)
*/
uint8_t i2c_write_word(uint8_t dev_addr, uint8_t reg_addr, uint16_t data) {
    return i2c_write_words(dev_addr, reg_addr, 1, &data);
}

/** Write multiple bytes to an 8-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr First register address to write to
* @param length Number of bytes to write
* @param data Buffer to copy new data from
* @return Status of operation (true = success)
*/
uint8_t i2c_write_bytes(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint8_t* data) {
    uint8_t status = 0;
    BUFFER[0] = reg_addr;
    twi_write_to(dev_addr, BUFFER, length, true, false);
    twi_write_to(dev_addr, data, length, true, true);
    return status == 0;
}

/** Write multiple words to a 16-bit device register.
* @param dev_addr I2C slave device address
* @param reg_addr First register address to write to
* @param length Number of words to write
* @param data Buffer to copy new data from
* @return Status of operation (true = success)
*/
uint8_t i2c_write_words(uint8_t dev_addr, uint8_t reg_addr, uint8_t length, uint16_t* data) {
    uint8_t status = 0;
    BUFFER[0] = reg_addr;
    twi_write_to(dev_addr, BUFFER, 1, true, false);
    for (uint8_t i = 0; i < length; i++) {
        BUFFER[i] = (uint8_t)(data[i] >> 8);// send MSB
        BUFFER[i++] = (uint8_t)(data[i]);// send LSB
    }
    twi_write_to(dev_addr, BUFFER, length * 2, true, true);
    return status == 0;
}
