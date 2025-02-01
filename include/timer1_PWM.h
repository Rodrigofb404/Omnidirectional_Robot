#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// ======================================================================
// 0 - Fast PWM 8-bit
// 1 - Phase Corrected 8-bit (TOP = 0x00FF)
// 2 - Phase Corrected (TOP = ICR1)
// 3 - Phase Corrected (TOP = OCR1A) (OC1B Disconnected)
// ======================================================================

void timer1_PWM_mode (int8_t mode) {

    // (WGM13 & WGM12 & WGM11 &  WGM10) -> LOW
    TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
    TCCR1B &= ~((1 << WGM12) | (1 << WGM13));

    switch (mode) {
        case 0:
            TCCR1A |= (1 << WGM10);
            TCCR1B |= (1 << WGM12);
            break;
        case 1:
            TCCR1A |= (1 << WGM10);
            break;
        case 2:
            TCCR1A |= (1 << WGM11);
            TCCR1B |= (1 << WGM13);
            break;
        case 3:
            TCCR1A |= (1 << WGM11) | (1 << WGM10);
            TCCR1B |= (1 << WGM13);
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

void timer1_PWM_invert_mode (int8_t mode, int8_t start_OC1B) {

    // (COM1A0 & COM1A1 & COM1B0 & COM1B1) -> LOW
    TCCR1A &= ~((1 << COM1A0) | (1 << COM1A1) | (1 << COM1B0) | (1 << COM1B1));

    if (mode == 0) {
        TCCR1A |= (1 << COM1A1);
        if (start_OC1B != 0)
        {
            TCCR1A |= (1 << COM1B1);
        }
    } else if (mode == 1) {
        TCCR1A |= (1 << COM1A0) | (1 << COM1A1);
        if (start_OC1B != 0)
        {
            TCCR1A |= (1 << COM1B0) | (1 << COM1B1);
        }
    }
}

// ======================================================================
// 0 - Clock
// 1 - Clock / 8
// 2 - Clock / 64
// 3 - Clock / 256
// 4 - Clock / 1024
// ======================================================================

void timer1_prescaler (int8_t mode) {

    // (CS10 & CS11 & CS12) -> LOW
    TCCR1B &= ~((1 << CS10) | (1 << CS11) | (1 << CS12));

    switch (mode) {
        case 0:
            TCCR1B |= (1 << CS01);
            break;
        case 1:
            TCCR1B |= (1 << CS11);
            break;
        case 2:
            TCCR1B |= (1 << CS11) | (1 << CS10);
            break;
        case 3:
            TCCR1B |= (1 << CS12);
            break;
        case 4:
            TCCR1B |= (1 << CS12) | (1 << CS10);
            break;
        default:
            // Valores inválidos
            break;
    }
}

// ======================================================================
// Set PWM Value
// ======================================================================

void timer1_PWM_value (uint16_t PWM_valueA, uint16_t PWM_valueB) {
    OCR1A = PWM_valueA;
    OCR1B = PWM_valueB;
}

//=======================================================================
// Calculate the right value to get the desired time interruption interval
// ======================================================================
uint16_t calc_compare_value(float tempo_desejado, uint32_t frequencia, uint16_t prescaler) {
    uint16_t OCR_value = (uint16_t) ceil((tempo_desejado * frequencia) / prescaler) - 1;
    return OCR_value;
}

//=======================================================================
// 0 - Interruption when compare match A in Timer2 occurs
// 1 - Interruption when compare match B in Timer2 occurs
// ======================================================================

void config_CTC1 (int8_t mode) {

    TCCR1A = 0x00;
    TCCR1B = 0x00;

    // Select CTC mode
    TCCR1B |=  (1 << WGM12);
    
    // Clock / 256
    TCCR1B |=  (1 << CS12);

    switch (mode) {
        case 0:
            TIMSK1 |= (1 << OCIE1A);
            OCR1A = calc_compare_value(0.2, 16000000, 256);
            break;
        case 1:
            TIMSK1 |= (1 << OCIE1B);
            OCR1B = calc_compare_value(0.2, 16000000, 256);
            break;
        default:
            break;
    }

    sei(); // Enables global interruption
}
