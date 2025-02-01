#include <avr/io.h>

#define FIRST_5_BITS 0b00011111
#define PC0_PC1_PC2_PC3 0b00001111
// ======================================================================
// Configura os pinos utilizados para os encoders
// ======================================================================
void encoders_io_config () {

    DDRB &= ~((1 << DDB0));
    DDRC &= ~(PC0_PC1_PC2_PC3);  // PC0...PC3 -> INPUT
    DDRD &= ~((1 << DDD7));

    PORTB &= ~((1 << PORTB0)); 
    PORTD &= ~((1 << PORTD7)); 
    PORTC &= ~(PC0_PC1_PC2_PC3);
}

// ======================================================================
// Configura os pinos utilizados para os timers
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

void h_bridge_config() {
    DDRD = 0b00110111; // PD0...PD5 -> OUTPUT
    DDRB |= (1 << PB1);
}   

void IO_init() {
    h_bridge_config();
    encoders_io_config();
    timer_io_config();
}