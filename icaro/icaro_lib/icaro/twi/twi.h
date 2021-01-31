#ifndef __TWI_H_
#define __TWI_H_

#define TRUE 1
#define FALSE 0

#ifndef TWI_FREQ
#define TWI_FREQ 100000L
#endif

#ifndef TWI_BUFFER_LENGTH
#define TWI_BUFFER_LENGTH 32
#endif

#define TWI_READY 0
#define TWI_MRX   1
#define TWI_MTX   2
#define TWI_SRX   3
#define TWI_STX   4

void twi_init();
uint8_t twi_read_from(uint8_t address, uint8_t *data, uint8_t length, uint8_t send_stop);
uint8_t twi_write_to(uint8_t address, uint8_t *data, uint8_t length, uint8_t wait, uint8_t send_stop);
void twi_reply(uint8_t ack);
void twi_stop(void);
void twi_release_bus(void);

#endif