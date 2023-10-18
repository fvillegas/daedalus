#include "timer.h"

void timer_init(void)
{
  DDRB |= (1 << PB1);
  TCNT1 = 0;
  ICR1 = 39999;
  TCCR1A =  (1 << COM1A1) | (0 << COM1A0);
  TCCR1A |=  (1 << WGM11) | (0 << WGM10);
  TCCR1B = (1 << WGM13) | (1 << WGM12);
  TCCR1B |= (0 << CS12) | (1 << CS11) | ( 0 << CS10 );
  OCR1A = 4000;
}
