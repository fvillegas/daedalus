#include <avr/io.h>
#include <util/delay.h>
#include "timer.h"

int main(void)
{ 
  DDRB |= (1<<PB0);
  
  timer_init();
  while (1){
    PORTB |= (1<<PB0);
    _delay_ms(500);
    PORTB &= ~(1<<PB0);
    _delay_ms(500);
  }

  return 0;
}
