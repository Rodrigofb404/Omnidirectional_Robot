#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <timer0_PWM.h>
#include <timer1_PWM.h>
#include <timer2_PWM.h>
#include <timerX_PID.h>

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
