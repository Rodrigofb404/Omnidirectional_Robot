#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

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
        case 3:
            TCCR2A &= ~(1 << WGM20);
            TCCR2A |= (1 << WGM21);
            TCCR2B &= ~(1 << WGM22);
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
        if (start_OC2B != 0)
        {
            TCCR2A |= (1 << COM2B1);
        }
        
    } else if(mode == 1) {
        TCCR2A |= (1 << COM2A0) | (1 << COM2A1);
        if (start_OC2B != 0)
        {
            TCCR2A |= (1 << COM2B0) | (1 << COM2B1);
        }
        
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

void timer2_PWM_value (uint8_t PWM_valueA, uint8_t PWM_valueB) {
    OCR2A = PWM_valueA;
    OCR2B = PWM_valueB;
}

//=======================================================================
// 0 - Interruption when overflow in Timer2 occurs
// 1 - Interruption when compare match A in Timer2 occurs
// 2 - Interruption when compare match B in Timer2 occurs
// ======================================================================

void config_CTC2 (int8_t mode) {
    // Enables Global Interruption
    SREG |= (1 << SREG_I);

    //==========Teste===============
    DDRB |= (1 << DDB5);
    //==============================

    switch (mode) {
    case 0:
        TIMSK2 |= (1 << TOIE2);
        break;
    case 1:
        TIMSK2 |= (1 << OCIE2A);
        break;
    case 2:
        TIMSK2 |= (1 << OCIE2B);
        break;
    default:
        break;
    }
}


//=======================================================================
// Calculate the right value to get the desired time interruption interval
// ======================================================================
uint16_t calc_compare_value(float tempo_desejado, uint32_t frequencia, uint16_t prescaler) {
    uint16_t OCR_value = (uint16_t) ceil((tempo_desejado * frequencia) / prescaler) - 1;
    return OCR_value;
}


volatile uint8_t countertimer = 0; // If necessary, change the size of countertimer

void timer2_interruption (uint8_t countertimer_compare) {
    if (TIFR2 & (1 << OCF2A)) {
        // Clearing flag
        TIFR2 |= (1 << OCF2A); 
        countertimer++;
        if (countertimer >= countertimer_compare) {
            countertimer = 0;
            PINB |= (1 << PINB5);
        }
    }
}
