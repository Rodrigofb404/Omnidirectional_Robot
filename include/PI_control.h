#include <avr/io.h>
#include <avr/interrupt.h>

float kp = 0.25, ki = 0.05;
float error = 0, previous_error = 0;
float p = 0, i = 0, d = 0;
float pi = 0, previous_pi = 0;

// Controle PI
void pi_control(int16_t rpm, int16_t rpm_ideal) {
    // Calcula erro
    error = rpm_ideal - rpm;

    // PI terms
    p = kp * (error - previous_error);
    i = ki * (error + previous_error);
    //d = kd * (erro - erro_anterior);

    // PI output
    pi = p + i;

    // Limits output between 0 e 255
    if (pi < 0) pi = 0;
    if (pi > 255) pi = 255;

    // Saves previous_error and previous_ouput
    previous_error = error;
    previous_pi = pi;

    return pi;
}