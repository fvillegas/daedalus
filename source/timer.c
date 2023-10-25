#include <avr/io.h>
#include "timer.h"

void timer_init(void)
{
  // Setup timers to generate Fast PWM
  // Timer 0
  TCNT0 = 0;

  // COM0A1 and COM0B1 connect pins to PWM to timer0  
  TCCR0A = (1 << COM0A1) | (1 << COM0B1);
  // Set timer0 to Fast PWM
  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  // Set prescaler to 64
  TCCR0B = (1 << CS01) | (1 << CS00);

  // Timer 1
  TCNT1 = 0;

  // COM1A1 and COM1B1 connect pins to PWM to timer0  
  TCCR1A = (1 << COM1A1) | (1 << COM1B1);
  // Set timer1 to Phase correct 8 bit
  TCCR1A |= (1 << WGM10);
  // Set prescaler to 64
  TCCR1B = (1 << CS11) | (1 << CS10);

  // Timer 2
  TCNT2 = 0;

  // COM2A1 and COM2B1 connect pins to PWM to timer0  
  TCCR2A = (1 << COM2A1) | (1 << COM2B1);
  // Set timer1 to Phase correct 8 bit
  TCCR2A |= (1 << WGM20);
  // Set prescaler to 64
  TCCR2B = (1 << CS22); 

  OCR0A = 100;
  OCR0B = 100;
  OCR1A = 100;
  OCR1B = 100;
  OCR2A = 100;
  OCR2B = 100;
}
