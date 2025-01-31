#include <stdio.h>
#include <avr/io.h>


//==========================================
//   Encoder in Proteus has 3 signals
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

    // Any change on any enabled PCINTn pin will cause an interrupt
    PCICR |= ((1 << PCIE0) | (1 << PCIE1) | (1 << PCIE2));

    // Pin change interrupt for especific inputs is enabled
    PCMSK0 |= (1 << PCINT0); // PB0
    PCMSK1 |= (1 << PCINT8); // PC0
    PCMSK2 |= (1 << PCINT23); // PD7

}

void encoder0_interruption (void) {
    if (PCIFR & (1 << PCIF0)) {
        // Clears Interruption Flag
        PCIFR |= (1 << PCIF0);
        counter0++;
    }
}

void encoder1_interruption (void) {
    if (PCIFR & (1 << PCIF1)) {
        // Clears Interruption Flag
        PCIFR = (1 << PCIF1);
        counter1++;          
    }
}

void encoder2_interruption (void) {
    if (PCIFR & (1 << PCIF2)) {
        // Clears Interruption Flag
        PCIFR |= (1 << PCIF2);
        counter2++;
    }
}