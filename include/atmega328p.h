#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>
#include <timer0_PWM.h>
#include <timer1_PWM.h>
#include <timer2_PWM.h>
#include <PID_control.h>
#include <encoder.h>
#include <io.h>
#include <h_bridge.h>

#define SET_ALL 0xFF
#define CLEAR_ALL 0x00

int16_t speed_motor1;
int16_t speed_motor2;
int16_t speed_motor3;

// ============================= Timer 0 ================================
//       PWM MODE           |      INVERT MODE       |     PRESCALER
// ====================================================================== 
//    0 - Fast PWM          | 0 - None-inverted mode | 0 - Clock
//    1 - Phase PWM (0xFF)  | 1 - Inverted mode      | 1 - Clock / 8
//    2 - Phase PWM (OCR0A) |                        | 2 - Clock / 64
//                          |                        | 3 - Clock / 256
//                          |                        | 4 - Clock / 1024
// ======================================================================

void config_timer0_PWM (int8_t pwm_mode, int8_t invert_mode, int8_t prescaler_mode, uint8_t PWM_valueA, uint8_t PWM_valueB) {
    timer0_PWM_value(PWM_valueA, PWM_valueB);
    timer0_PWM_mode(pwm_mode);
    timer0_PWM_invert_mode(invert_mode, PWM_valueB);
    timer0_prescaler(prescaler_mode);
}

// ===================================== Timer 1 ==============================================
//          PWM MODE              |        INVERT MODE         |         PRESCALER
// ============================================================================================ 
//   0 - Fast PWM 8-bit           | 0 - None-inverted mode     | 0 - Clock
//   1 - Phase PWM 8-bit (0x00FF) | 1 - Inverted mode          | 1 - Clock / 8
//   2 - Phase PWM (ICR1)         |                            | 2 - Clock / 64
//   3 - Phase PWM (OCR0A)        |                            | 3 - Clock / 256
//                                |                            | 4 - Clock / 1024
// ============================================================================================

void config_timer1_PWM (int8_t pwm_mode, int8_t invert_mode, int8_t prescaler_mode, uint16_t PWM_valueA, uint16_t PWM_valueB) {
    timer1_PWM_value(PWM_valueA, PWM_valueB);
    timer1_PWM_mode(pwm_mode);
    timer1_PWM_invert_mode(invert_mode, PWM_valueB);
    timer1_prescaler(prescaler_mode);
}

// ================================== Timer 2 =======================================
//        PWM MODE          |         INVERT MODE           |     PRESCALER
// ==================================================================================
//    0 - Fast PWM          | 0 - None-inverted mode (OC2A) | 0 - Clock
//    1 - Phase PWM (0xFF)  | 1 - Inverted mode      (OC2A) | 1 - Clock / 8
//    2 - Phase PWM (OCR0A) | 2 - None-inverted mode (OC2B) | 2 - Clock / 64
//                          | 3 - Inverted mode      (OC2B) | 3 - Clock / 256
//                          |                               | 4 - Clock / 1024
// ==================================================================================

void config_timer2_PWM (int8_t pwm_mode, int8_t invert_mode, int8_t prescaler_mode, uint8_t PWM_valueA, uint8_t PWM_valueB) {
    timer2_PWM_value(PWM_valueA, PWM_valueB);
    timer2_PWM_mode(pwm_mode);
    timer2_PWM_invert_mode(invert_mode, PWM_valueB);
    timer2_prescaler(prescaler_mode);
}

void encoder (int8_t config_mode) {
    config_encoder();
    config_CTC1(config_mode);
}

// ======================================================================
// PPS = Pulses per Second
// PPR = Pulses per rotation
// ======================================================================
float rpm_calc(int16_t PPS, int16_t PPR) {
    float rpm = (PPS * 2000.0) / (PPR);
    return rpm;
}

uint8_t pid_controlM1(float rpm, float rpm_ideal);
uint8_t pid_controlM2(float rpm, float rpm_ideal);
uint8_t pid_controlM3(float rpm, float rpm_ideal);
void calc_coeficients_pid();

#define R 0.04        // Radius = 40mm
#define L 0.10        // Distance between the robot center of mass and the wheels  
#define MAX_rpm 262   // Max RPM that motors can reach with Max PWM

#define ALPHA1 0.0      // Wheel 1 (0°)
#define ALPHA2 2.0944   // Wheel 2 (120°)
#define ALPHA3 4.1888   // Wheel 3 (240°)



// ======================================================================
// Vx - X-axis velocity
// Vy - Y-axis velocity
// W  - θ velocity
// desired_RPM_Mx - necessary RPM to reach the desired velocity
// ======================================================================
void Kinematics(float Vx, float Vy, float W, float *desired_RPM_M1, float *desired_RPM_M2, float *desired_RPM_M3) {
    float v1 = (-Vx * sin(ALPHA1) + Vy * cos(ALPHA1) + L * W) / R;
    float v2 = (-Vx * sin(ALPHA2) + Vy * cos(ALPHA2) + L * W) / R;
    float v3 = (-Vx * sin(ALPHA3) + Vy * cos(ALPHA3) + L * W) / R;

    // Conversion of rad/s to RPM
    *desired_RPM_M1 = (v1 * 60.0) / (2 * M_PI * R);
    *desired_RPM_M2 = (v2 * 60.0) / (2 * M_PI * R);
    *desired_RPM_M3 = (v3 * 60.0) / (2 * M_PI * R);

    *desired_RPM_M1 = (*desired_RPM_M1 > MAX_rpm) ? MAX_rpm : (*desired_RPM_M1 < -MAX_rpm) ? -MAX_rpm : *desired_RPM_M1;
    *desired_RPM_M2 = (*desired_RPM_M2 > MAX_rpm) ? MAX_rpm : (*desired_RPM_M2 < -MAX_rpm) ? -MAX_rpm : *desired_RPM_M2;
    *desired_RPM_M3 = (*desired_RPM_M3 > MAX_rpm) ? MAX_rpm : (*desired_RPM_M3 < -MAX_rpm) ? -MAX_rpm : *desired_RPM_M3;
}

// ======================================================================
// By pushing the buttons we select the direction of the robot
// Angular reference it's not implemented
// ======================================================================
void direction_control(uint8_t FORWARD_btn, uint8_t LEFT_btn, uint8_t RIGHT_btn, uint8_t BACKWARD_btn, uint8_t CW_btn, uint8_t CCW_btn, 
                       float *desired_RPM_M1, float *desired_RPM_M2, float *desired_RPM_M3) {

    float Vx = 0.0, Vy = 0.0, W = 0.0;

    if (FORWARD_btn) {
        Vy = 0.04;       // 0.04 m/s para frente
    } 
    else if (LEFT_btn) {
        Vx = -0.04;      // 0.04 m/s para esquerda
    } 
    else if (RIGHT_btn) {
        Vx = 0.04;       // 0.04 m/s para direita
    } 
    else if (BACKWARD_btn) {
        Vy = -0.04;      // 0.04 m/s para trás
    } 
    else if (CW_btn) {
        W = 1;           // 1 rad/s no sentido horário
    } 
    else if (CCW_btn) {
        W = -1;          // 1 rad/s no sentido anti-horário
    }

    Kinematics(Vx, Vy, W, desired_RPM_M1, desired_RPM_M2, desired_RPM_M3);
}