#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <timer0_PWM.h>
#include <timer1_PWM.h>
#include <timer2_PWM.h>
#include <timerX_PID.h>

#define SENSOR0_PIN PD2  // Encoder motor 0 (INT0)
#define SENSOR1_PIN PD3  // Encoder motor 1 (INT1)
#define SENSOR2_PIN PC2  // Encoder motor 2 (PCINT10)

int main() {
    // Configurações iniciais
    config_timer0_pwm(1,0,1,255,64)
    config_timer1_pwm(1,0,1,255,128)
    config_timer2_pwm(1,0,1,255,210)

    config_timer1();
    config_pwm();

    // Configurar interrupção externa no INT0 (PD2)
    EICRA = (1 << ISC01);  // Interrupção na borda de descida
    EIMSK = (1 << INT0);   // Habilitar INT0

    sei();  // Habilitar interrupções globais

    while (1) {
        pid_control();  // Executa o controle PID continuamente
        _delay_ms(10);  // Pequeno atraso para estabilidade
    }

    return 0;
}

