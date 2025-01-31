#include <avr/io.h>

#define SET_FIRST_5_BITS 0b00011111
// ======================================================================
// Configura os pinos utilizados para os encoders
// ======================================================================
void encoders_io_config () {
    // PB0 (PCINT0), PC0 (PCINT8), PD7 (PCINT23) as inputs
    DDRB &= ~((1 << DDB0));
    DDRC &= ~((1 << DDC0));
    DDRD &= ~((1 << DDD7));

    PORTB &= ~((1 << PORTB0)); 
    PORTC &= ~((1 << PORTC0));
    PORTD &= ~((1 << PORTD7));

}

// ======================================================================
// Configura os pinos utilizados para os timers
// ======================================================================
void timer_io_config() {

    // Timer 0 
    DDRD |= (1 << DDD6); // PD6
    DDRD |= (1 << DDD5); // PD5

    // Timer 1
    DDRB |= (1 << DDB1); // PB1
    DDRB |= (1 << DDB2); // PB2

    // Timer 2 REVISAR
    // DDRB |= (1 << DDB3); // PB3
    // DDRD |= (1 << DDD3); // PD3
}

void h_bridge_config() {
    DDRD = SET_FIRST_5_BITS; // PD0...PD5 -> OUTPUT
}

void IO_init() {
    h_bridge_config();
    encoders_io_config();
    timer_io_config();
}