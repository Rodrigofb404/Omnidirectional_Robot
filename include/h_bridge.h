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
        PORTD |= (1 << PD0);  // IN1 -> HIGH
        PORTD &= ~(1 << PD1); // IN2 -> LOW 
    } else {
        PORTD &= ~(1 << PD0); // IN1 -> LOW 
        PORTD |= (1 << PD1);  // IN2 -> HIGH
    }
}

// ======================================================================
// Stop the rotation of the MOTOR
// ======================================================================
void stop_motor1 () {
    PORTD &= ~(1 << PD0); // IN1 -> LOW 
    PORTD &= ~(1 << PD1);  // IN2 -> LOW
}

// ======================================================================
// 0 - Clockwise rotation 
// 1 - Counter Clockwise rotation 
// ======================================================================
void motor2_rotation (int8_t direction) {
    if (direction == 0)
    {
        PORTD |= (1 << PD2);  // IN3 -> HIGH
        PORTD &= ~(1 << PD3); // IN4 -> LOW 
    } else {
        PORTD &= ~(1 << PD2); // IN3 -> LOW 
        PORTD |= (1 << PD3);  // IN4 -> HIGH
    }
}

// ======================================================================
// Stop the rotation of the MOTOR1
// ======================================================================
void stop_motor2 () {
    PORTD &= ~(1 << PD2); // IN1 -> LOW 
    PORTD &= ~(1 << PD3);  // IN2 -> LOW
}

// ======================================================================
// 0 - Clockwise rotation 
// 1 - Counter Clockwise rotation 
// ======================================================================
void motor3_rotation (int8_t direction) {
    if (direction == 0)
    {
        PORTD |= (1 << PD4);  // IN1 -> HIGH
        PORTD &= ~(1 << PD5); // IN2 -> LOW 
    } else {
        PORTD &= ~(1 << PD4); // IN1 -> LOW 
        PORTD |= (1 << PD5);  // IN2 -> HIGH
    }
}

// ======================================================================
// Stop the rotation of the MOTOR3
// ======================================================================
void stop_motor3 () {
    PORTD &= ~(1 << PD4); // IN1 -> LOW 
    PORTD &= ~(1 << PD5);  // IN2 -> LOW
}