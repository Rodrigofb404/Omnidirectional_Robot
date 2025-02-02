#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>


float Kp = 0.72, Ki = 0.0145, Kd = 0.0119; // BEST coef values for 0.3 ms verify and 90 PPR
float error0 = 0, error1 = 0, error2 = 0;
float pid0 = 0, pid1 = 0, pid2 = 0;
float Ts = 0.03;
uint8_t N = 20;
uint8_t pwm = 0;

float a0, a1, a2, b0, b1, b2;
float ku1, ku2, ke0, ke1, ke2;

void calc_coeficients_pid() {
    a0 = (1 + N * Ts);
    a1 = -(2 + N * Ts);
    a2 = 1;
    b0 = Kp * (1 + N * Ts) + Ki * Ts * (1 + N * Ts) + Kd * N;
    b1 = -(Kp * (2 + N * Ts) + Ki * Ts + 2 * Kd * N);
    b2 = Kp + Kd * N;

    ku1 = a1 / a0;
    ku2 = a2 / a0;
    ke0 = b0 / a0;
    ke1 = b1 / a0;
    ke2 = b2 / a0;
}

// Controle PID
int pid_control(int16_t rpm, int16_t rpm_ideal) {
    error2 = error1; error1 = error0; pid2 = pid1; pid1 = pid0; // Updates variables
    
    error0 = rpm_ideal - rpm; // Computes new error

    pid0 = -ku1 * pid1 - ku2 * pid2 + ke0 * error0 + ke1 * error1 + ke2 * error2; // Calculates pid

    // Limits output between 0 e 255
    if (pid0 < 0) pwm = 0;
    else if (pid0 > 255) pwm = 255;
    else  pwm = (uint8_t)pid0;

    return pwm;
}