#include <avr/io.h>

#define FIRST_5_BITS 0b00011111
#define PC0_PC1_PC2_PC3 0b00001111
// ======================================================================
// Configure the ENCODERS pins
// ======================================================================
void encoders_io_config () {

    DDRB &= ~((1 << DDB0));
    DDRC &= ~(PC0_PC1_PC2_PC3);  // PC0...PC3 -> INPUT
    DDRD &= ~((1 << DDD2));

    PORTB &= ~((1 << PORTB0)); 
    PORTD &= ~((1 << PORTD7)); 
    PORTC &= ~(PC0_PC1_PC2_PC3);
}

// ======================================================================
// Configure the TIMERX pins
// ======================================================================
void timer_io_config() {

    // Timer 0 
    DDRD |= (1 << DDD6); // PD6
    DDRD |= (1 << DDD5); // PD5

    // // Timer 1
    // DDRB |= (1 << DDB1); // PB1
    // DDRB |= (1 << DDB2); // PB2

    // Timer 2 REVISAR
    DDRB |= (1 << DDB3); // PB3
    DDRD |= (1 << DDD3); // PD3
}

// ======================================================================
// Configure the H-BRIDGE pins
// ======================================================================
void h_bridge_config() {
    DDRD = 0b00110011; // PD0...PD2, PD4 and PD5 -> OUTPUT
    DDRB |= (1 << PB1);
    DDRB |= (1 << PB0);
    DDRB |= (1 << PB5);
}   

// ======================================================================
// Configure the PUSH BUTTONS pins
// ======================================================================
void interface_config() {

    // FORWARD (INPUT pull-up)
    DDRD &= ~(1 << PD7); 
    PORTD |= (1 << PD7); 

    // LEFT (INPUT pull-up)
    DDRC &= ~(1 << PC4); 
    PORTC |= (1 << PC4); 

    // RIGHT (INPUT pull-up)
    DDRB &= ~(1 << PB2); 
    PORTB |= (1 << PB2); 

    // BACKWARD (INPUT pull-up)
    DDRB &= ~(1 << PB4); 
    PORTB |= (1 << PB4); 

    // CW (INPUT pull-up)
    DDRC &= ~(1 << PC5); 
    PORTC |= (1 << PC5); 
}

// ======================================================================
// Start all the pins configurations
// ======================================================================
void IO_init() {
    h_bridge_config();
    encoders_io_config();
    timer_io_config();
    interface_config();
}