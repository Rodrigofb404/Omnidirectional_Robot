#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// ======================================================================
// 0 - Fast PWM 
// 1 - Phase PWM (TOP = 0xFF) 
// 2 - Phase PWM (TOP = OCRA)
// ======================================================================

void timer2_PWM_mode (int8_t mode) {

    // (WGM20 & WGM21 & WGM22) -> LOW
    TCCR2A &= ~((1 << WGM20) | (1 << WGM21));
    TCCR2B &= ~(1 << WGM22);

    switch (mode) {
        case 0:
            TCCR2A |= (1 << WGM20) | (1 << WGM21);
            break;
        case 1:
            TCCR2A |= (1 << WGM20);
            break;
        case 2:
            TCCR2A |= (1 << WGM20);
            TCCR2B |= (1 << WGM22);
            break;
        default:
            // Valores inválidos
            break;
    }
}

// ======================================================================
// 0 - OC2A -> None-inverted mode (HIGH at bottom, LOW on match) 
// 1 - OC2A -> Inverted mode (LOW at bottom, HIGH on Match) 
// 2 - OC2B -> None-inverted mode (HIGH at bottom, LOW on match) 
// 3 - OC2B -> Inverted mode (LOW at bottom, HIGH on Match) 
// ======================================================================

void timer2_PWM_invert_mode (int8_t mode) {

    // (COM2A0 & COM2A1 & COM2B0 & COM2B1) -> LOW
    TCCR2A &= ~((1 << COM2A0) | (1 << COM2A1) | (1 << COM2B0) | (1 << COM2B1));

    switch (mode) {
        case 0:
            TCCR2A |= (1 << COM2A1);
            TCCR2A |= (1 << COM2B1);
            break;
        case 1:
            TCCR2A |= (1 << COM2A0) | (1 << COM2A1);
            TCCR2A |= (1 << COM2B0) | (1 << COM2B1);
            break;
        default:
            // Valores inválidos
            break;
    }
}

// ======================================================================
// 0 - Clock
// 1 - Clock / 8
// 2 - Clock / 64
// 3 - Clock / 256
// 4 - Clock / 1024
// ======================================================================

void timer2_prescaler (int8_t mode) {

    // (CS20 & CS21 & CS22) -> LOW
    TCCR2B &= ~((1 << CS20) | (1 << CS21) | (1 << CS22));

    switch (mode) {
        case 0:
            TCCR2B |= (1 << CS20);
            break;
        case 1:
            TCCR2B |= (1 << CS21);
            break;
        case 2:
            TCCR2B |= (1 << CS20) | (1 << CS21);
            break;
        case 3:
            TCCR2B |= (1 << CS22);
            break;
        case 4:
            TCCR2B |= (1 << CS20) | (1 << CS22);
            break;
        default:
            // Valores inválidos
            break;
    }
}

// ======================================================================
// Set PWM Value
// ======================================================================

void timer2_PWM_value (uint8_t PWM_valueA, uint8_t PWM_valueB) {
    OCR2A = PWM_valueA;
    OCR2B = PWM_valueB;
}
