#ifndef __PINS_H__
#define __PINS_H__

#include <avr/io.h>

/**
 * > output
 * < input
 * ~ pwm
 * + analog
 *     ___
 *   1|   |28
 *   2|   |27
 *   3|   |26
 *   4|   |25
 *   5|   |24
 *   6|   |23
 *   7|   |22
 *   8|   |21
 *   9|   |20
 *  10|   |19
 *  11|   |18
 *  12|   |17
 *  13|   |16
 *  14|   |15 > ~
 *     ---
 **/

void pins_init(void) {
  DDRB |= (1 << PB1);
  DDRB |= (1 << PB2);
  DDRB |= (1 << PB3);
  DDRD |= (1 << PD3);
  DDRD |= (1 << PD5);
  DDRD |= (1 << PD6);
}

#endif
