#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

float kp = 0.25, ki = 0.05;
float error = 0, previous_error = 0;
float p = 0, i = 0, d = 0;
float pi = 0, previous_pi = 0, pi_output;

// Controle PI
int pi_control(int16_t rpm, int16_t rpm_ideal) {
    //ki = kp * 0.1/(2*(kp/ki));
    // Calcula erro
    error = rpm_ideal - rpm;

    // PI terms
    p = kp * (error - previous_error);
    i = ki * (error + previous_error);
    //d = kd * (erro - erro_anterior);

    // PI output
    pi = p + i;
    pi_output = pi + previous_pi;

    // Limits output between 0 e 255
    if (pi_output < 0) pi_output = 0;
    else if (pi_output > 255) pi_output = 255;
    else pi_output = (uint8_t)pi_output;

    // Saves previous_error and previous_ouput
    previous_error = error;
    previous_pi = pi;

    return pi_output;
}