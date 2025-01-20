#include <stdio.h>
#include <avr/io.h>


//==========================================
// Encoder in Proteus has 3 signals
//------------------------------------------
//      Q1     |    Q2    | IDX
//  Direction of Rotation | Home Position
//==========================================

volatile uint64_t counter0 = 0;
volatile uint64_t counter1 = 0;
volatile uint64_t counter2 = 0;

void config_encoder (void) {
    // Enables Global Interruption
    SREG |= (1 << SREG_I);

    // Any change on any enabled PCINT14..8 pin will cause an interrupt
    PCICR |= (1 << PCIE1);

    // Pin change interrupt for especific inputs is enabled 
    PCMSK1 |= ((1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10));

    // PC0 (PCINT8), PC1 (PCINT9), PC2 (PCINT10) as inputs
    DDRC &= ~((1 << DDC0) | (1 << DDC1) | (1 << DDC2));
    PORTC &= ~((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2));

    // Teste PB4
    DDRB |= (1 << DDB4);
}

void encoder_interruption (void) {
    if (PCIFR & (1 << PCIF1)) {
        // Clears Interruption Flag
        PCIFR |= (1 << PCIF1);
        
        //PINC0 == 1? 0000_0000 & 0000_0001
        if (PINC & (1 << PINC0)) {
            counter0++;
            PINB |= (1 << PINB4);
        } 
        if (PINC & (1 << PINC1)) {
            counter1++;
        } 
        if (PINC & (1 << PINC2)) {
            counter2++;
        }    
    }
}

