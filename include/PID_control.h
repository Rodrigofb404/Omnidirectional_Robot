#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

// BEST coef values for 0.3 ms verify and 90 PPR
// float Kp = 0.9, Ki = 0.0434, Kd = 0.019;  

// BEST coef values for 0.3 ms verify and 300 PPR
float Kp = 0.9, Ki = 0.065, Kd = 0.019; //  Proportional, Integral and Derivative gains  

float error01 = 0, error11 = 0, error21 = 0; // Previous and actual errors of Motor1

float pid01 = 0, pid11 = 0, pid21 = 0; // Previous and actual PID of Motor1

float error02 = 0, error12 = 0, error22 = 0; // Previous and actual PID of Motor2

float pid02 = 0, pid12 = 0, pid22 = 0; // Previous and actual PID of Motor2

float error03 = 0, error13 = 0, error23 = 0; // Previous and actual errors of Motor3

float pid03 = 0, pid13 = 0, pid23 = 0; // Previous and actual PID of Motor3

float Ts = 0.03; // Sampling Time

uint8_t N = 20; // Filter coefficients

uint8_t pwm = 0; // PID output

float a0, a1, a2; // Controller Input coefficients

float b0, b1, b2; // Controller Output coefficients

float ku1, ku2, ke0, ke1, ke2; // PID coefficients

void calc_coeficients_pid() {
    //==========================================
    // Calculates Controller Input coefficients
    //==========================================
    a0 = (1 + N * Ts);
    a1 = -(2 + N * Ts);
    a2 = 1;

    //===========================================
    // Calculates Controller Output coefficients
    //===========================================
    b0 = Kp * (1 + N * Ts) + Ki * Ts * (1 + N * Ts) + Kd * N;
    b1 = -(Kp * (2 + N * Ts) + Ki * Ts + 2 * Kd * N);
    b2 = Kp + Kd * N;

    //=============================
    // Calculates PID coefficients
    //=============================
    ku1 = a1 / a0;
    ku2 = a2 / a0;
    ke0 = b0 / a0;
    ke1 = b1 / a0;
    ke2 = b2 / a0;
}

//=====================
// PID Controller
// Each motor has one
//=====================
uint8_t pid_controlM1(float rpm, int16_t rpm_ideal) {
    //===================
    // Updates variables
    //===================
    error21 = error11; error11 = error01; pid21 = pid11; pid11 = pid01; 
    
    //====================
    // Computes new error
    //====================
    error01 = rpm_ideal - rpm;

    //============================================================
    // Calculates pid | Difference equation for controller output
    //============================================================
    pid01 = -ku1 * pid11 - ku2 * pid21 + ke0 * error01 + ke1 * error11 + ke2 * error21; 

    //===============================
    // Limits output between 0 e 255
    //===============================
    if (pid01 < 0) pwm = 0;
    else if (pid01 > 255) pwm = 255;
    else  pwm = (uint8_t)pid01;

    return pwm;
}

uint8_t pid_controlM2(float rpm, int16_t rpm_ideal) {
    //===================
    // Updates variables
    //===================
    error22 = error12; error12 = error02; pid22 = pid12; pid12 = pid02;
    
    //====================
    // Computes new error
    //====================
    error02 = rpm_ideal - rpm;

    //============================================================
    // Calculates pid | Difference equation for controller output
    //============================================================
    pid02 = -ku1 * pid12 - ku2 * pid22 + ke0 * error02 + ke1 * error12 + ke2 * error22;

    //===============================
    // Limits output between 0 e 255
    //===============================
    if (pid02 < 0) pwm = 0;
    else if (pid02 > 255) pwm = 255;
    else  pwm = (uint8_t)pid02;

    return pwm;
}

uint8_t pid_controlM3(float rpm, int16_t rpm_ideal) {
    //===================
    // Updates variables
    //===================
    error23 = error13; error13 = error03; pid23 = pid13; pid13 = pid03;

    //====================
    // Computes new error
    //====================    
    error03 = rpm_ideal - rpm;

    //============================================================
    // Calculates pid | Difference equation for controller output
    //============================================================
    pid03 = -ku1 * pid13 - ku2 * pid23 + ke0 * error03 + ke1 * error13 + ke2 * error23;

    //===============================
    // Limits output between 0 e 255
    //===============================
    if (pid03 < 0) pwm = 0;
    else if (pid03 > 255) pwm = 255;
    else  pwm = (uint8_t)pid03;

    return pwm;
}