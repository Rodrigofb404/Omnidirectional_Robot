#include <atmega328p.h>

#define SET_ALL 0xFF
#define CLEAR_ALL 0x00

void PWMconfig (void) {
  /*TIMER0*/
  TCCR0A |= (1 << WGM02); // OCRA is TOP
  TCCR0A |= (1 << WGM00); // Phase correct
  TCCR0A |= (1 << COM0A1); // Clear OC0A on compare match when up-counting. Set OC0A on compare match when down-counting
  TCCR0A |= (1 << COM0B1); // Clear OC0B on compare match when up-counting. Set OC0B on compare match when down-counting
  TCCR0B |= (1 << CS01); // (clk I/O)/8 (from prescaler)
  
  DDRD |= (1 << DDD6); // PD6 as an external output for the Timer0

  /*TIMER1*/
  TCCR1A |= (1 << WGM13);
  TCCR1A |= (1 << WGM11);
  TCCR1A |= (1 << WGM10); // PWM, phase correct ; OCR1A is TOP
  TCCR1A |= (1 << COM1A1); // Clear OC1A/OC1B on compare match when up-counting. Set OC1A/OC1B on compare match when down counting.
  TCCR1B |= (1 << CS11); // (clk I/O)/8 (from prescaler)

  DDRD |= (1 << DDB1); // PD1 as an external output for the Timer1

  /*TIMER2*/
  TCCR2A |= (1 << WGM22);
  TCCR2A |= (1 << WGM20); // PWM, phase correct. OCRA is TOP
  TCCR2A |= (1 << COM2A1); // Clear OC0A on compare match when up-counting. Set OC0A on compare match when down-counting
  TCCR2A |= (1 << COM2B1); //Clear OC0B on compare match when up-counting. Set OC0B on compare match when down-counting
  TCCR2B |= (1 << CS21); // (clk I/O)/8 (from prescaler)

  DDRD |= (1 << DDB3); // PD3 as an external output for the Timer0
}

int main (void) {
  while (1)
  {
    // TESTE
  } 
}