#include <atmega328p.h>

int main (void) {
  config_timer0_PWM(1, 0, 1, 255, 64);
  config_timer1_PWM(1, 0, 1, 255, 128);
  config_timer2_PWM(3, 2, 4, 255, 0);
  encoder(1);
  while (1)
  {
    interruption_routine(30); // ~1s delay
  } 
}
