#include <atmega328p.h>

//#define SENSOR0_PIN PD2  // Encoder motor 0 (INT0)
//#define SENSOR1_PIN PD3  // Encoder motor 1 (INT1)
//#define SENSOR2_PIN PC2  // Encoder motor 2 (PCINT10)


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
