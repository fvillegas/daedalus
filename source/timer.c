#include <avr/io.h>
#include "timer.h"

void timer_init(void)
{
  // Setup timers to generate Fast PWM
  TCNT1 = 0; // Reset counter to 0
  ICR1 = 39999;
  TCCR1A =  (1 << COM1A1) | (0 << COM1A0);
  TCCR1A |=  (1 << COM1B1) | (0 << COM1B0);
  TCCR1A |=  (1 << WGM11) | (0 << WGM10);

  TCCR1B = (1 << WGM13) | (1 << WGM12);
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);

  OCR1A = 2000;
  OCR1B = 20000;
}
