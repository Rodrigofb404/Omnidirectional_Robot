#include "kinematic.h"
#include <util/delay.h>

int main() {
   // Configura os pinos de entrada com pull-up interno - CORRIGIDO
    DDRC &= ~((1 << FRENTE_PIN) | (1 << TRAS_PIN) | (1 << ESQUERDA_PIN)| (1 << DIREITA_PIN) | (1 << ROTACAO_HORARIA_PIN) | (1 << ROTACAO_ANTIHORARIA_PIN));
    PORTC |= ((1 << FRENTE_PIN) | (1 << TRAS_PIN) | (1 << ESQUERDA_PIN)| (1 << DIREITA_PIN) | (1 << ROTACAO_HORARIA_PIN) | (1 << ROTACAO_ANTIHORARIA_PIN));

    setup_motor_drivers();
    setup_pwm_timer0();
    setup_pwm_timer1();
    conf_encoder();

    while (1) {
        float vx = 0;
        float vy = 0;
        float omega = 0;

        if (!(PINC & (1 << FRENTE_PIN))) { //CORRIGIDO
            vx = 1;
        } else if (!(PINC & (1 << TRAS_PIN))) { //CORRIGIDO
            vx = -1;
        }

        if (!(PINC & (1 << ESQUERDA_PIN))) { //CORRIGIDO
            vy = 1;
        } else if (!(PINC & (1 << DIREITA_PIN))) { //CORRIGIDO
            vy = -1;
        }

        if (!(PINC & (1 << ROTACAO_HORARIA_PIN))) { //CORRIGIDO
          omega = 1;
        }else if (!(PINC & (1 << ROTACAO_ANTIHORARIA_PIN))){ //CORRIGIDO
          omega = -1;
        }

        float omega1 = (-vx + (2 * vy) / sqrt(3.0) + omega) / R;
        float omega2 = (-vx - (2 * vy) / sqrt(3.0) + omega) / R;
        float omega3 = (2 * vx + omega) / R;

        uint8_t pwm1 = (uint8_t)(fabs(omega1) * (MAX_PWM / 10));
        uint8_t pwm2 = (uint8_t)(fabs(omega2) * (MAX_PWM / 10));
        uint8_t pwm3 = (uint8_t)(fabs(omega3) * (MAX_PWM / 10));

        pwm1 = (pwm1 > MAX_PWM) ? MAX_PWM : pwm1;
        pwm2 = (pwm2 > MAX_PWM) ? MAX_PWM : pwm2;
        pwm3 = (pwm3 > MAX_PWM) ? MAX_PWM : pwm3;

        set_motor_speed(pwm1, omega1, pwm2, omega2, pwm3, omega3);
    }
    return 0;
}