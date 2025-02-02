#include <stdio.h>
#include <avr/io.h>
#include <math.h>
//==========================================
//   Encoder in Proteus has 3 signals
//------------------------------------------
//      Q1     |    Q2    | IDX
//  Direction of Rotation | Home Position
//==========================================

// ======================================================================
// Configure the pin interruptions for encoders
// ======================================================================
void config_encoder (void) {
    // Enables Global Interruption
    SREG |= (1 << SREG_I);

    
    PCICR |= (1 << PCIE1);                  // Rising and falling edge interrupt on PCINT8...PCINT14
    EICRA |= ((1 << ISC00) | (1 << ISC01)); // Rising Edge interrupt INT0
    EICRA |= ((1 << ISC10) | (1 << ISC11)); // Rising Edge interrupt INT1
    EIMSK |= ((1 << INT0) | (1 << INT1));   // Enable interrupts for PD2 and PD3

    
    PCMSK1 |= (1 << PCINT8); // Enables PCinterrupt for PC0
}