#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define SET_ALL 0xFF
#define CLEAR_ALL 0x00
// ====================== CONFIG REGISTRADORES PWM ======================
                            
// ================================ Timer 0 =============================

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

void timer0_PWM_invert_mode (int8_t mode) {

    // (COM0A0 & COM0A1) -> LOW
    TCCR0A &= ~((1 << COM0A0) | (1 << COM0A1));

    if (mode == 0) {
        TCCR0A |= (1 << COM0A1);
    } else if (mode == 1)
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
//       PWM MODE           |      INVERT MODE       |     PRESCALER
// ====================================================================== 
//    0 - Fast PWM          | 0 - None-inverted mode | 0 - Clock
//    1 - Phase PWM (0xFF)  | 1 - Inverted mode      | 1 - Clock / 8
//    2 - Phase PWM (OCR0A) |                        | 2 - Clock / 64
//                          |                        | 3 - Clock / 256
//                          |                        | 4 - Clock / 1024
// ======================================================================

void config_timer0_PWM (int8_t pwm_mode, int8_t invert_mode, int8_t prescaler_mode) {
    timer0_PWM_mode(pwm_mode);
    timer0_PWM_invert_mode(invert_mode);
    timer0_prescaler(prescaler_mode);
}

// ================================ Timer 1 =============================

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

void timer1_PWM_invert_mode (int8_t mode) {

    // (COM1A0 & COM1A1 & COM1B0 & COM1B1) -> LOW
    TCCR1A &= ~((1 << COM1A0) | (1 << COM1A1) | (1 << COM1B0) | (1 << COM1B1));

    if (mode == 0) {
        TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
    } else if (mode == 1)
        TCCR1A |= (1 << COM1A0) | (1 << COM1A1);
        TCCR1A |= (1 << COM1B0) | (1 << COM1B1);
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

// ============================================================================================
//          PWM MODE              |        INVERT MODE         |         PRESCALER
// ============================================================================================ 
//   0 - Fast PWM 8-bit           | 0 - None-inverted mode     | 0 - Clock
//   1 - Phase PWM 8-bit (0x00FF) | 1 - Inverted mode          | 1 - Clock / 8
//   2 - Phase PWM (ICR1)         |                            | 2 - Clock / 64
//   3 - Phase PWM (OCR0A)        |                            | 3 - Clock / 256
//                                |                            | 4 - Clock / 1024
// ============================================================================================

void config_timer1_PWM (int8_t pwm_mode, int8_t invert_mode, int8_t prescaler_mode) {
    timer1_PWM_mode(pwm_mode);
    timer1_PWM_invert_mode(invert_mode);
    timer1_prescaler(prescaler_mode);
}

// ================================ Timer 2 =============================

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
    TCCR0A &= ~((1 << COM2A0) | (1 << COM2A1) | (1 << COM2B0) | (1 << COM2B1));

    switch (mode) {
        case 0:
            TCCR2A |= (1 << COM2A1);
            break;
        case 1:
            TCCR2A |= (1 << COM2A0) | (1 << COM2A1);
            break;
        case 2:
            TCCR2A |= (1 << COM2B1);
            break;
        case 3:
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

// ==================================================================================
//       PWM MODE           |            INVERT MODE        |     PRESCALER
// ==================================================================================
//    0 - Fast PWM          | 0 - None-inverted mode (OC2A) | 0 - Clock
//    1 - Phase PWM (0xFF)  | 1 - Inverted mode      (OC2A) | 1 - Clock / 8
//    2 - Phase PWM (OCR0A) | 2 - None-inverted mode (OC2B) | 2 - Clock / 64
//                          | 3 - Inverted mode      (OC2B) | 3 - Clock / 256
//                          |                               | 4 - Clock / 1024
// ==================================================================================

void config_timer2_PWM (int8_t pwm_mode, int8_t invert_mode, int8_t prescaler_mode) {
    timer2_PWM_mode(pwm_mode);
    timer2_PWM_invert_mode(invert_mode);
    timer2_prescaler(prescaler_mode);
}