#include <avr/io.h>
#include "timer.h"

void timer_init(void)
{
  // Setup timers to generate Fast PWM
  // Timer 0
  TCNT0 = 0;
  
  TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01);
  TCCR0A |= (1 << WGM00);
  TCCR0B = (1 << CS01) | (1 << CS00);

  // Timer 1
  TCNT1 = 0;

  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
  TCCR1B = (1 << CS11) | (1 << CS10);

  // Timer 2
  TCNT2 = 0;
  
  TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM20);
  TCCR2B = (1 << CS22); 
  
  OCR0A = 100;
  OCR0B = 100;
  OCR1A = 100;
  OCR1B = 100;
  OCR2A = 100;
  OCR2B = 100;
}
