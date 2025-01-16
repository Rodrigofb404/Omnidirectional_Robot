// ----------- ATmega328p Registers Config. ---------------
#include <avr/io.h>
#include <util/delay.h>
// ---------------- Interrupts handling -----------------

void enable_interruptions(void) {
    SREG |= (1 << 7);  // SET Bit-7 of SREG 
}

void disable_interruptions(void) {
    SREG &= ~(1 << 7); // CLEAR Bit-7 of SREG                  
}

//
