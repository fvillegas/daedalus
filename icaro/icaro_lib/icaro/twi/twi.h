/************************************************************************/
/* COPY FROM ARDUINO.CC PROJECT                                         */
/************************************************************************/

/*
twi.h - TWI/I2C library for Wiring & Arduino
Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Modified 2020 by Greyson Christoforo (grey@christoforo.net) to implement timeouts
*/

/*
* Modifications
* remove arduino dependencies
* rename functions and variables
*/

#ifndef __TWI_H_
#define __TWI_H_

#include <inttypes.h>

#ifndef TWI_FREQ
#define TWI_FREQ 100000L
#endif

#ifndef TWI_BUFFER_LENGTH
#define TWI_BUFFER_LENGTH 32
#endif

#define TWI_READY 0
#define TWI_MRX 1
#define TWI_MTX 2
#define TWI_SRX 3
#define TWI_STX 4

void twi_init(void);
void twi_disable(void);
void twi_set_address(uint8_t);
void twi_set_frequency(uint32_t);
uint8_t twi_read_from(uint8_t, uint8_t *, uint8_t, uint8_t);
uint8_t twi_writeTo(uint8_t, uint8_t *, uint8_t, uint8_t, uint8_t);
uint8_t twi_transmit(const uint8_t *, uint8_t);
void twi_attach_slave_rx_event(void (*)(uint8_t *, int));
void twi_attach_slave_tx_event(void (*)(void));
void twi_reply(uint8_t);
void twi_stop(void);
void twi_release_bus(void);
void twi_set_timeout_in_micros(uint32_t, uint8_t);
void twi_handle_timeout(uint8_t);
uint8_t twi_manage_timeout_flag(uint8_t);

#endif
