#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <timer0_PWM.h>
#include <timer1_PWM.h>
#include <timer2_PWM.h>
#include <timerX_PID.h>

<<<<<<< HEAD
#define SENSOR0_PIN PD2  // Encoder motor 0 (INT0)
#define SENSOR1_PIN PD3  // Encoder motor 1 (INT1)
#define SENSOR2_PIN PC2  // Encoder motor 2 (PCINT10)

int main() {
    // Configuração inicial
    DDRD &= ~(1 << SENSOR0_PIN);   // Configura PD2 como entrada (encoder)
    EICRA = (1 << ISC01);          // Interrupção na borda de descida
    EIMSK = (1 << INT0);           // Habilitar INT0

    config_timer1();               // Configura Timer1 para cálculo de RPM
    timer0_PWM_mode(0);            // Configura Timer0 no modo Fast PWM
    timer0_PWM_invert_mode(0);     // Configura modo não invertido
    timer0_prescaler(2);           // Prescaler para Timer0 (64)
    timer0_PWM_value(0);           // Duty cycle inicial (0)

    sei();  // Habilita interrupções globais

    while (1) {
        pid_control();  // Executa controle PID continuamente
        _delay_ms(10);  // Pequeno atraso para estabilidade
    }

    return 0;
}
=======
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
>>>>>>> 5c5611bbe6e2005b525dadb31127e4550606a3cc
