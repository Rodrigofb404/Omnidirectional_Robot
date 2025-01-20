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
    // Configurar pinos dos sensores como entrada
    DDRD &= ~((1 << SENSOR0_PIN) | (1 << SENSOR1_PIN));  // Sensores 0 e 1
    DDRC &= ~(1 << SENSOR2_PIN);  // Sensor 2

    // Habilitar pull-ups internos
    PORTD |= (1 << SENSOR0_PIN) | (1 << SENSOR1_PIN);
    PORTC |= (1 << SENSOR2_PIN);

    // Configurar interrupções externas para os encoders
    EICRA = (1 << ISC01) | (1 << ISC11);  // Bordas de descida para INT0 e INT1
    EIMSK = (1 << INT0) | (1 << INT1);    // Habilitar INT0 e INT1

    // Configurar interrupção de mudança de pino para PCINT10 (Sensor 2)
    PCICR |= (1 << PCIE1);  // Habilitar interrupções de PCINT[14:8]
    PCMSK1 |= (1 << PCINT10);

    // Configurar timers para PWM
    config_timer0_PWM(0, 0, 2, 0);  // Motor 0
    config_timer1_PWM(0, 0, 2, 0);  // Motor 1
    config_timer2_PWM(0, 0, 2, 0);  // Motor 2

    // Configurar timer para cálculo de RPM
    config_timer1();

    sei();  // Habilitar interrupções globais

    while (1) {
        pid_control();  // Executar controle PID para todos os motores
        _delay_ms(10);  // Delay para estabilização
    }
}
