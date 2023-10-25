#include <avr/io.h>
#include <util/delay.h>
#include "timer.h"
#include "pins.h"

int main(void)
{ 
  pins_init();
  timer_init();

  while (1){
  }

  return 0;
}
