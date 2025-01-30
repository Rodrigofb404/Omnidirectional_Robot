#include <atmega328p.h>

int main (void) {
  config_timer0_PWM(1, 0, 1, 255, 64);
  config_timer1_PWM(1, 0, 1, 255, 128);
  config_timer2_PWM(1, 0, 1, 255, 210);
  encoder();    
  while (1)
  {
      interruption_routine();
  } 
}
