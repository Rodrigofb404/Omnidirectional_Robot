#include <atmega328p.h>

int main (void) {
  config_timer0_PWM(1, 0, 1, 50);
  config_timer1_PWM(1, 0, 1, 128);
  config_timer2_PWM(1, 0, 1, 210);
  while (1)
  {
    // teste
  } 
}