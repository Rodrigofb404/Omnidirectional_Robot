#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define SET_ALL 0xFF
#define CLEAR_ALL 0x00
// ====================== CONFIG REGISTRADORES PWM ======================

// ======================================================================
// 0 - Fast PWM 
// 1 - Phase PWM (TOP = 0xFF) 
// 2 - Phase PWM (TOP = OCRA)
// ======================================================================

void timer0_PWM_mode(int8_t mode) {

    //  (WGM00 & WGM01 & WGM02) -> LOW
    TCCR0A &= ~((1 << WGM00) | (1 << WGM01));
    TCCR0B &= ~(1 << WGM02);

    switch (mode) {
        case 0:
            TCCR0A |= (1 << WGM00) | (1 << WGM01);
            break;
        case 1:
            TCCR0A |= (1 << WGM00);
            break;
        case 2:
            TCCR0A |= (1 << WGM00);
            TCCR0B |= (1 << WGM02);
            break;
        default:
            // Valores inválidos
            break;
    }
}

// ======================================================================
// 0 - None-inverted mode (HIGH at bottom, LOW on match) 
// 1 - Inverted mode (LOW at bottom, HIGH on Match) 
// ======================================================================

void timer0_PWM_invert_mode(int8_t mode) {

    // (COM0A0 & COM0A1) -> LOW
    TCCR0A &= ~((TCCR0A << COM0A0) | (TCCR0A << COM0A1));

    if (mode == 1) {
        TCCR0A |= (1 << COM0A1);
    } else if (mode == 2)
        TCCR0A |= (1 << COM0A0) | (1 << COM0A1);
}

// ======================================================================
// 0 - Clock
// 1 - Clock / 8
// 2 - Clock / 64
// 3 - Clock / 256
// 4 - Clock / 1024
// ======================================================================

void timer0_prescaler (int8_t mode) {

    // (CS00 & CS01 & CS02) -> LOW
    TCCR0B &= ~((1 << CS00) | (1 << CS01) | (1 << CS02));

    switch (mode) {
        case 0:
            TCCR0B |= (1 << CS00);
            break;
        case 1:
            TCCR0B |= (1 << CS01);
            break;
        case 2:
            TCCR0B |= (1 << CS00) | (1 << CS01);
            break;
        case 3:
            TCCR0B |= (1 << CS02);
            break;
        case 4:
            TCCR0B |= (1 << CS00) | (1 << CS02);
            break;
        default:
            // Valores inválidos
            break;
    }
}

// ======================================================================
// Top function for config the Timer0 PWM
// ======================================================================

void timer0_PWM_config (int8_t pwm_mode, int8_t invert_mode, int8_t prescaler_mode) {
    timer0_PWM_mode(pwm_mode);
    timer0_PWM_invert_mode(invert_mode);
    timer0_prescaler(prescaler_mode);
}

