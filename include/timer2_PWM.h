#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

// ======================================================================
// 0 - Fast PWM 
// 1 - Phase PWM (TOP = 0xFF) 
// 2 - Phase PWM (TOP = OCRA)
// 3 - Compare Output Mode, non-PWM (TOP = OCRA)
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
// 0 - OC2A -> None-inverted mode (HIGH at bottom, LOW on match) / OC2B -> None-inverted mode (HIGH at bottom, LOW on match)
// 1 - OC2A -> Inverted mode (LOW at bottom, HIGH on Match) / OC2B -> Inverted mode (LOW at bottom, HIGH on Match)
// Any other value - OC2A/OC2B is disconnected
// ======================================================================

void timer2_PWM_invert_mode (int8_t mode, int8_t start_OC2B) {

    // (COM2A0 & COM2A1 & COM2B0 & COM2B1) -> LOW
    TCCR2A &= ~((1 << COM2A0) | (1 << COM2A1) | (1 << COM2B0) | (1 << COM2B1));

    if (mode == 0) {
        TCCR2A |= (1 << COM2A1);

        if (start_OC2B >= 0) TCCR2A |= (1 << COM2B1);
     
    } else if(mode == 1) {
        TCCR2A |= (1 << COM2A0) | (1 << COM2A1); 
        
        if (start_OC2B >= 0) TCCR2A |= (1 << COM2B0) | (1 << COM2B1);     
    }
}

// ======================================================================
// 0 - Clock
// 1 - Clock / 8
// 2 - Clock / 32
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
            TCCR2B |= ((1 << CS22) | (1 << CS21));
            break;
        case 4:
            TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);
            break;
        default:
            // Valores inválidos
            break;
    }
}

// ======================================================================
// Set PWM Value / TOP
// ======================================================================

void timer2_PWM_value (int16_t PWM_valueA, int16_t PWM_valueB) {
    if (OCR2A + PWM_valueA >= 0 && OCR2B + PWM_valueB >= 0) {
        OCR2A += PWM_valueA;
        OCR2B += PWM_valueB;
    }
}
