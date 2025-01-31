#include <atmega328p.h>
// #include "avr8-stub.h"

int main (void) {
  IO_init();
  
  // debug_init();
  config_timer0_PWM(0, 0, 4, 100, 0);
  config_timer1_PWM(0, 0, 4 , 200, 255);
  encoder(1);

  motor1_rotation(0);
  motor2_rotation(0);
  motor3_rotation(1);
  while (1)
  {
    interruption_routine(30); // ~1s delay
  } 
}
