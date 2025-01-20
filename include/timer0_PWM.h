#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// ======================================================================
// 0 - Fast PWM 
// 1 - Phase PWM (TOP = 0xFF) 
// 2 - Phase PWM (TOP = OCRA)
// ======================================================================

void timer0_PWM_mode (int8_t mode) {

    //  (WGM00 & WGM01 & WGM02) -> LOW
    TCCR0A &= ~((1 << WGM00) | (1 << WGM01));
    TCCR0B &= ~(1 << WGM02);

    switch (mode) {
        case 0:
            TCCR0A |= ((1 << WGM00) | (1 << WGM01));
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

void timer0_PWM_invert_mode (int8_t mode) {

    // (COM0A0 & COM0A1 & COM0B0 & COM0B1) -> LOW
    TCCR0A &= ~((1 << COM0A0) | (1 << COM0A1) | (1 << COM0B0) | (1 << COM0B1));

    if (mode == 0) {
        TCCR0A |= (1 << COM0A1);
        TCCR0A |= (1 << COM0B1);
    } else if (mode == 1) {
        TCCR0A |= ((1 << COM0A0) | (1 << COM0A1));
        TCCR0A |= ((1 << COM0B0) | (1 << COM0B1));
    }
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
            TCCR0B |= ((1 << CS00) | (1 << CS01));
            break;
        case 3:
            TCCR0B |= (1 << CS02);
            break;
        case 4:
            TCCR0B |= ((1 << CS00) | (1 << CS02));
            break;
        default:
            // Valores inválidos
            break;
    }
}

// ======================================================================
// Set PWM Value
// ======================================================================

void timer0_PWM_value (uint8_t PWM_valueA, uint8_t PWM_valueB) {
    OCR0A = PWM_valueA;
    OCR0B = PWM_valueB;
} 
