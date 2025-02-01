#ifndef KINEMATIC_H
#define KINEMATIC_H

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <avr/interrupt.h>

// Definições dos pinos (ATmega328P) - CORRIGIDO
#define FRENTE_PIN PC0
#define TRAS_PIN PC1
#define ESQUERDA_PIN PC2
#define DIREITA_PIN PC3
#define ROTACAO_HORARIA_PIN PC4
#define ROTACAO_ANTIHORARIA_PIN PC5

#define MOTOR1_PIN PD5 // OC0B
#define MOTOR2_PIN PD6 // OC0A
#define MOTOR3_PIN PB1 // OC1A

#define MOTOR1_IN1 PD2
#define MOTOR1_IN2 PD3
#define MOTOR2_IN3 PD4
#define MOTOR2_IN4 PD7
#define MOTOR3_IN1 PB0
#define MOTOR3_IN2 PB2

// Definições dos pinos dos encoders - CORRIGIDO (PB7 duplicado corrigido)
#define ENCODER0_A PC6 // PCINT14
#define ENCODER0_B PB7
#define ENCODER1_A PB6 // PCINT6
#define ENCODER1_B PB5 // CORRIGIDO - PB5 agora é usado para ENCODER1_B
#define ENCODER2_A PD0 // PCINT16
#define ENCODER2_B PD1

// Constantes
#define R 0.05
#define MAX_PWM 255
#define PI 3.14159265358979323846

// Implementação das funções DENTRO do cabeçalho
static volatile uint64_t counter0 = 0;
static volatile uint64_t counter1 = 0;
static volatile uint64_t counter2 = 0;

void conf_encoder(void) {
    SREG |= (1 << SREG_I);

    // Habilita interrupções para os pinos dos encoders - CORRIGIDO
    PCICR |= (1 << PCIE0) | (1 << PCIE2); // PCINT6 está no PCIE0, PCINT14 e PCINT16 no PCIE2
    PCMSK0 |= (1 << PCINT6); // PB6
    PCMSK2 |= (1 << PCINT14) | (1 << PCINT16); // PC6 e PD0

    // Configura os pinos dos encoders como entrada - CORRIGIDO
    DDRC &= ~(1 << DDC6);
    DDRB &= ~((1 << DDB6) | (1 << DDB5) | (1 << DDB7)); // PB5 e PB7 adicionados
    DDRD &= ~((1 << DDD0) | (1 << DDD1));
    PORTB |= (1 << PORTB5) | (1 << PORTB7); // Pull-up para PB5 e PB7
}

ISR(PCINT0_vect) { // Interrupção para o encoder 1 (PB6)
    PCIFR |= (1 << PCIF0);
    static uint8_t last_state1 = 0;
    uint8_t current_state1 = (PINB & (1 << PINB6)) | ((PINB & (1 << PINB5)) >> 1); // Lê PB6 e PB5 - CORRIGIDO
    if (current_state1 != last_state1) {
        if ((last_state1 == 0x0 && current_state1 == 0x1) ||
            (last_state1 == 0x1 && current_state1 == 0x3) ||
            (last_state1 == 0x3 && current_state1 == 0x2) ||
            (last_state1 == 0x2 && current_state1 == 0x0)) {
            counter1++; // Rotação em um sentido
        } else {
            counter1--; // Rotação no sentido oposto
        }
        last_state1 = current_state1;
    }
}

ISR(PCINT2_vect) { // Interrupção para os encoders 0 e 2 (PC6 e PD0)
    PCIFR |= (1 << PCIF2);
    static uint8_t last_state0 = 0;
    uint8_t current_state0 = (PINC & (1 << PINC6)) | ((PINB & (1 << PINB7)) << 5); // Lê PC6 e PB7
    if (current_state0 != last_state0) {
        if ((last_state0 == 0x0 && current_state0 == 0x1) ||
            (last_state0 == 0x1 && current_state0 == 0x3) ||
            (last_state0 == 0x3 && current_state0 == 0x2) ||
            (last_state0 == 0x2 && current_state0 == 0x0)) {
            counter0++; // Rotação em um sentido
        } else {
            counter0--; // Rotação no sentido oposto
        }
        last_state0 = current_state0;
    }

    static uint8_t last_state2 = 0;
    uint8_t current_state2 = (PIND & (1 << PIND0)) | ((PIND & (1 << PIND1)) >> 1); // Lê PD0 e PD1
    if (current_state2 != last_state2) {
        if ((last_state2 == 0x0 && current_state2 == 0x1) ||
            (last_state2 == 0x1 && current_state2 == 0x3) ||
            (last_state2 == 0x3 && current_state2 == 0x2) ||
            (last_state2 == 0x2 && current_state2 == 0x0)) {
            counter2++; // Rotação em um sentido
        } else {
            counter2--; // Rotação no sentido oposto
        }
        last_state2 = current_state2;
    }
}

void setup_pwm_timer0(void) {
  DDRD |= (1 << MOTOR1_PIN) | (1 << MOTOR2_PIN);

  // Fast PWM mode
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
  TCCR0B |= (1 << CS01); // Prescaler 8
}

void setup_pwm_timer1(void) {
  DDRB |= (1 << MOTOR3_PIN);

  // Fast PWM, 8-bit
  TCCR1A |= (1 << WGM10) | (1 << COM1A1);
  TCCR1B |= (1 << CS11); // Prescaler 8
}

void setup_motor_drivers(void) {
    // Configura os pinos de controle dos motores como saída
    DDRD |= (1 << MOTOR1_IN1) | (1 << MOTOR1_IN2) | (1 << MOTOR2_IN3) | (1 << MOTOR2_IN4);
    DDRB |= (1 << MOTOR3_IN1) | (1 << MOTOR3_IN2);
}

// Implementação corrigida da função set_motor_speed
void set_motor_speed(uint8_t pwm1, float omega1, uint8_t pwm2, float omega2, uint8_t pwm3, float omega3) {
    if (pwm1 > 0) { // Verifica se há PWM para aplicar direção
        if (omega1 >= 0) {
            PORTD |= (1 << MOTOR1_IN1);
            PORTD &= ~(1 << MOTOR1_IN2);
        } else {
            PORTD &= ~(1 << MOTOR1_IN1);
            PORTD |= (1 << MOTOR1_IN2);
        }
    } else { // Se PWM é 0, para o motor
        PORTD &= ~(1 << MOTOR1_IN1);
        PORTD &= ~(1 << MOTOR1_IN2);
    }
    OCR0B = pwm1;

    if (pwm2 > 0) { // Verifica se há PWM para aplicar direção
        if (omega2 >= 0) {
            PORTD |= (1 << MOTOR2_IN3);
            PORTD &= ~(1 << MOTOR2_IN4);
        } else {
            PORTD &= ~(1 << MOTOR2_IN3);
            PORTD |= (1 << MOTOR2_IN4);
        }
    } else { // Se PWM é 0, para o motor
        PORTD &= ~(1 << MOTOR2_IN3);
        PORTD &= ~(1 << MOTOR2_IN4);
    }
    OCR0A = pwm2;

    if (pwm3 > 0) { // Verifica se há PWM para aplicar direção
        if (omega3 >= 0) {
            PORTB |= (1 << MOTOR3_IN1);
            PORTB &= ~(1 << MOTOR3_IN2);
        } else {
            PORTB &= ~(1 << MOTOR3_IN1);
            PORTB |= (1 << MOTOR3_IN2);
        }
    } else { // Se PWM é 0, para o motor
        PORTB &= ~(1 << MOTOR3_IN1);
        PORTB &= ~(1 << MOTOR3_IN2);
    }
    OCR1A = pwm3;
}

#endif