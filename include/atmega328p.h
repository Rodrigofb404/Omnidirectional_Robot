#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <timer0_PWM.h>
#include <timer1_PWM.h>
#include <timer2_PWM.h>
#include <PID_control.h>
#include <encoder.h>
#include <io.h>
#include <h_bridge.h>

#define SET_ALL 0xFF
#define CLEAR_ALL 0x00

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