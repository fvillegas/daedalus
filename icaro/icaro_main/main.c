#include <avr/io.h>
#include <avr/interrupt.h>

#include "icaro/timer/timer.h"
#include "icaro/twi/twi.h"
#include "icaro/twi/i2cdevlib.h"
#include "icaro/imu.h"

#define STATUS_LED PB5

#ifdef DEBUG
#include "icaro/uart/uart.h"
char DEBUG_BUFFER[150] = {0};
#endif

long last = 0L;
long now = 0L;

void setup(void)
{
    DDRB |= (1 << STATUS_LED);
    
    init_millis(F_CPU);
    twi_init();
    
    sei();
    
    #ifdef DEBUG
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
    uart_puts("setup finish\n");
    #endif
}

int main(void)
{
    setup();
    while (1) 
    {
        now = millis();
        
        if ((now - last) > 100) {
            PORTB ^= (1 << STATUS_LED);
            #ifdef DEBUG
            
            uint8_t data[50] = {0};
            i2c_read_bytes(IMU_TWI_ADDRESS, 0, IMU_REGISTER_LENGTH, data);
            
            #endif
            last = now;
        }
    }
}

