/*
 * timer.h
 */ 


#ifndef TIMER_H_
#define TIMER_H_

void timer_init(void);
void delay_microseconds(unsigned int us);
void delay(unsigned long ms);
unsigned long micros(void);
unsigned long millis(void);

#endif /* TIMER_H_ */