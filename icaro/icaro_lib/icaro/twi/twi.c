#include <stdbool.h>
#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "twi.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

static volatile uint8_t twi_state;
static volatile uint8_t twi_slarw;
static volatile uint8_t twi_send_stop;    // should the transaction end with a stop
static volatile uint8_t twi_in_rep_start; // in the middle of a repeated start

static void (*twi_on_slave_transmit)(void);
static void (*twi_on_slave_receive)(uint8_t *, int);

static uint8_t twi_master_buffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_master_buffer_index;
static volatile uint8_t twi_master_buffer_length;

static uint8_t twi_tx_buffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_tx_buffer_index;
static volatile uint8_t twi_tx_buffer_length;

static uint8_t twi_rx_buffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_rx_buffer_index;

static volatile uint8_t twi_error;

void twi_init()
{
    // initialize twi prescaler and bit rate
    cbi(TWSR, TWPS0);
    cbi(TWSR, TWPS1);
    TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;

    // enable twi module, acks, and twi interrupt
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}

uint8_t twi_read_from(uint8_t address, uint8_t *data, uint8_t length, uint8_t send_stop)
{
    uint8_t i;
    // ensure data will fit into buffer
    if (TWI_BUFFER_LENGTH < length)
    {
        return 0;
    }

    // wait until twi is ready, become master receiver
    while (TWI_READY != twi_state)
    {
        continue;
    }

    twi_state = TWI_MRX;
    twi_send_stop = send_stop;
    // reset error state (0xFF.. no error occured)
    twi_error = 0xFF;

    // initialize buffer iteration vars
    twi_master_buffer_index = 0;
    twi_master_buffer_length = length - 1; // This is not intuitive, read on...
    // On receive, the previously configured ACK/NACK setting is transmitted in
    // response to the received byte before the interrupt is signalled.
    // Therefor we must actually set NACK when the _next_ to last byte is
    // received, causing that NACK to be sent in response to receiving the last
    // expected byte of data.

    // build sla+w, slave device address + w bit
    twi_slarw = TW_READ;
    twi_slarw |= address << 1;

    if (TRUE== twi_in_rep_start)
    {
        // if we're in the repeated start state, then we've already sent the start,
        // (@@@ we hope), and the TWI statemachine is just waiting for the address byte.
        // We need to remove ourselves from the repeated start state before we enable interrupts,
        // since the ISR is ASYNC, and we could get confused if we hit the ISR before cleaning
        // up. Also, don't enable the START interrupt. There may be one pending from the
        // repeated start that we sent ourselves, and that would really confuse things.
        twi_in_rep_start = false; // remember, we're dealing with an ASYNC ISR
        do
        {
            TWDR = twi_slarw;
        } while (TWCR & _BV(TWWC));
        TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE); // enable INTs, but not START
    }
    else
    {
        // send start condition
        TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);
    }
    
    // wait for read operation to complete
    while (TWI_MRX == twi_state)
    {
        continue;
    }

    if (twi_master_buffer_index < length)
        length = twi_master_buffer_index;

    // copy twi buffer to data
    for (i = 0; i < length; ++i)
    {
        data[i] = twi_master_buffer[i];
    }

    return length;
}

uint8_t twi_write_to(uint8_t address, uint8_t *data, uint8_t length, uint8_t wait, uint8_t send_stop)
{
    uint8_t i;

    // ensure data will fit into buffer
    if (TWI_BUFFER_LENGTH < length)
    {
        return 1;
    }

    // wait until twi is ready, become master transmitter
    while (TWI_READY != twi_state)
    {
        continue;
    }
    
    twi_state = TWI_MTX;
    twi_send_stop = send_stop;
    // reset error state (0xFF.. no error occured)
    twi_error = 0xFF;

    // initialize buffer iteration vars
    twi_master_buffer_index = 0;
    twi_master_buffer_length = length;

    // copy data to twi buffer
    for (i = 0; i < length; ++i)
    {
        twi_master_buffer[i] = data[i];
    }

    // build sla+w, slave device address + w bit
    twi_slarw = TW_WRITE;
    twi_slarw |= address << 1;

    // if we're in a repeated start, then we've already sent the START
    // in the ISR. Don't do it again.
    //
    if (TRUE == twi_in_rep_start)
    {
        // if we're in the repeated start state, then we've already sent the start,
        // (@@@ we hope), and the TWI statemachine is just waiting for the address byte.
        // We need to remove ourselves from the repeated start state before we enable interrupts,
        // since the ISR is ASYNC, and we could get confused if we hit the ISR before cleaning
        // up. Also, don't enable the START interrupt. There may be one pending from the
        // repeated start that we sent outselves, and that would really confuse things.
        twi_in_rep_start = FALSE; // remember, we're dealing with an ASYNC ISR
        do
        {
            TWDR = twi_slarw;
        } while (TWCR & _BV(TWWC));
        TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE); // enable INTs, but not START
    }
    else
    {
        // send start condition
        TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE) | _BV(TWSTA); // enable INTs
    }
    // wait for write operation to complete
    while (wait && (TWI_MTX == twi_state))
    {
        continue;
    }

    if (twi_error == 0xFF)
        return 0; // success
    else if (twi_error == TW_MT_SLA_NACK)
        return 2; // error: address send, nack received
    else if (twi_error == TW_MT_DATA_NACK)
        return 3; // error: data send, nack received
    else
        return 4; // other twi error
}

void twi_reply(uint8_t ack)
{
    // transmit master read ready signal, with or without ack
    if (ack) {
        TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
    }
    else {
        TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
    }
}

void twi_stop(void)
{
    // send stop condition
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);
    // wait for stop condition to be exectued on bus
    // TWINT is not set after a stop condition!
    while (TWCR & _BV(TWSTO))
    {
        continue;
    }
    // update twi state
    twi_state = TWI_READY;
}

void twi_release_bus(void)
{
    // release bus
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);
    // update twi state
    twi_state = TWI_READY;
}

ISR(TWI_vect)
{
    switch (TW_STATUS)
    {
    // All Master
    case TW_START:     // sent start condition
    case TW_REP_START: // sent repeated start condition
        TWDR = twi_slarw;
        twi_reply(TRUE);
        break;

    // Master Transmitter
    case TW_MT_SLA_ACK:  // slave receiver acked address
    case TW_MT_DATA_ACK: // slave receiver acked data
        // if there is data to send, send it, otherwise stop
        if (twi_master_buffer_index < twi_master_buffer_length)
        {
            // copy data to output register and ack
            TWDR = twi_master_buffer[twi_master_buffer_index++];
            twi_reply(TRUE);
        }
        else
        {
            if (twi_send_stop)
            {
                twi_stop();
            }
            else
            {
                twi_in_rep_start = TRUE; // we're gonna send the START
                // don't enable the interrupt. We'll generate the start, but we
                // avoid handling the interrupt until we're in the next transaction,
                // at the point where we would normally issue the start.
                TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
                twi_state = TWI_READY;
            }
        }
        break;
    case TW_MT_SLA_NACK: // address sent, nack received
        twi_error = TW_MT_SLA_NACK;
        twi_stop();
        break;
    case TW_MT_DATA_NACK: // data sent, nack received
        twi_error = TW_MT_DATA_NACK;
        twi_stop();
        break;
    case TW_MT_ARB_LOST: // lost bus arbitration
        twi_error = TW_MT_ARB_LOST;
        twi_release_bus();
        break;

    // Master Receiver
    case TW_MR_DATA_ACK: // data received, ack sent
        // put byte into buffer
        twi_master_buffer[twi_master_buffer_index++] = TWDR;
        // fall through
    case TW_MR_SLA_ACK: // address sent, ack received
        // ack if more bytes are expected, otherwise nack
        if (twi_master_buffer_index < twi_master_buffer_length)
        {
            twi_reply(TRUE);
        }
        else
        {
            twi_reply(FALSE);
        }
        break;
    case TW_MR_DATA_NACK: // data received, nack sent
        // put final byte into buffer
        twi_master_buffer[twi_master_buffer_index++] = TWDR;
        if (twi_send_stop)
        {
            twi_stop();
        }
        else
        {
            twi_in_rep_start = TRUE; // we're gonna send the START
            // don't enable the interrupt. We'll generate the start, but we
            // avoid handling the interrupt until we're in the next transaction,
            // at the point where we would normally issue the start.
            TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
            twi_state = TWI_READY;
        }
        break;
    case TW_MR_SLA_NACK: // address sent, nack received
        twi_stop();
        break;
    // TW_MR_ARB_LOST handled by TW_MT_ARB_LOST case

    // Slave Receiver
    case TW_SR_SLA_ACK:            // addressed, returned ack
    case TW_SR_GCALL_ACK:          // addressed generally, returned ack
    case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
    case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
    case TW_SR_DATA_ACK:           // data received, returned ack
    case TW_SR_GCALL_DATA_ACK:     // data received generally, returned ack
    case TW_SR_STOP:               // stop or repeated start condition received
    case TW_SR_DATA_NACK:          // data received, returned nack
    case TW_SR_GCALL_DATA_NACK:    // data received generally, returned nack

    // Slave Transmitter
    case TW_ST_SLA_ACK:          // addressed, returned ack
    case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
    case TW_ST_DATA_ACK:         // byte sent, ack returned
    case TW_ST_DATA_NACK:        // received nack, we are done
    case TW_ST_LAST_DATA:        // received ack, but we are done already!

    // All
    case TW_NO_INFO: // no state information
        break;
    case TW_BUS_ERROR: // bus error, illegal stop/start
        twi_error = TW_BUS_ERROR;
        twi_stop();
        break;
    }
}