#include <avr/io.h>


// ======================================================================
//            Turn the MOTOR on and set the direction
// ----------------------------------------------------------------------
// 0 - Clockwise rotation 
// 1 - Counter Clockwise rotation 
// ======================================================================
void motor1_rotation (int8_t direction) {
    if (direction == 0)
    {
        PORTD |= (1 << PD0);  // IN1H1 -> HIGH
        PORTD &= ~(1 << PD1); // IN2H1 -> LOW 
    } else {
        PORTD &= ~(1 << PD0); // IN1H1 -> LOW 
        PORTD |= (1 << PD1);  // IN2H1 -> HIGH
    }
}

// ======================================================================
// Stop the rotation of the MOTOR
// ======================================================================
void stop_motor1 () {
    PORTD &= ~(1 << PD0); // IN1H1 -> LOW 
    PORTD &= ~(1 << PD1);  // IN2H1 -> LOW
}

// ======================================================================
// 0 - Clockwise rotation 
// 1 - Counter Clockwise rotation 
// ======================================================================
void motor2_rotation (int8_t direction) {
    if (direction == 0)
    {
        PORTB |= (1 << PB0);  // IN3H1 -> HIGH
        PORTB &= ~(1 << PB1); // IN4H1 -> LOW 
    } else {
        PORTB &= ~(1 << PB0); // IN3H1 -> LOW 
        PORTB |= (1 << PB1);  // IN4H1 -> HIGH
    }
}

// ======================================================================
// Stop the rotation of the MOTOR1
// ======================================================================
void stop_motor2 () {
    PORTB &= ~(1 << PB0); // IN1H1 -> LOW 
    PORTB &= ~(1 << PD1);  // IN2H1 -> LOW
}

// ======================================================================
// 0 - Clockwise rotation 
// 1 - Counter Clockwise rotation 
// ======================================================================
void motor3_rotation (int8_t direction) {
    if (direction == 0)
    {
        PORTD |= (1 << PD4);  // IN1H2 -> HIGH
        PORTB &= ~(1 << PB5); // IN2H2 -> LOW 
    } else {
        PORTD &= ~(1 << PD4); // IN1H2 -> LOW 
        PORTB |= (1 << PB5);  // IN2H2 -> HIGH
    }
}

// ======================================================================
// Stop the rotation of the MOTOR3
// ======================================================================
void stop_motor3 () {
    PORTD &= ~(1 << PD4); // IN1H2 -> LOW 
    PORTB &= ~(1 << PB5);  // IN2H2 -> LOW
}