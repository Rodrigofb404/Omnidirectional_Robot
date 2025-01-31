#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

// Definições dos pinos de entrada para direção (exemplo)
#define FRENTE_PIN PIND1  // Pino IO1
#define TRAS_PIN PIND2    // Pino IO2
#define ESQUERDA_PIN PIND3 // Pino IO3
#define DIREITA_PIN PIND7  // Pino IO7
#define ROTACAO_HORARIA_PIN PINB1 //Pino IO9
#define ROTACAO_ANTIHORARIA_PIN PINB2 //Pino IO10

// Definições dos pinos de saída para os motores
#define MOTOR1_PIN PD0 // OC0A - Pino IO0
#define MOTOR2_PIN PC0 // OC0B - Pino AD0
#define MOTOR3_PIN PB0 // Pino IO8

// Definições de constantes
#define R 0.05 // Raio da roda em metros (exemplo)
#define MAX_PWM 255 // Valor máximo do PWM (8 bits)
#define PI 3.14159265358979323846

//Funções de configuração dos Timers e encoders(já definidas anteriormente)

void setup_pwm_timer0() {
  DDRB |= (1 << MOTOR1_PIN);
  DDRD |= (1 << MOTOR2_PIN);

  // Fast PWM mode
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
  TCCR0B |= (1 << CS01); // Prescaler 8
}


void set_motor_speed(uint8_t motor1_speed, uint8_t motor2_speed, uint8_t motor3_speed) {
  OCR0A = motor1_speed;
  OCR0B = motor2_speed;
  OCR0B_1 = motor3_speed;
}

int main() {
    // Configura os pinos de entrada com pull-up interno
    DDRB &= ~((1 << FRENTE_PIN) | (1 << TRAS_PIN) | (1 << ESQUERDA_PIN)| (1 << DIREITA_PIN) | (1 << ROTACAO_HORARIA_PIN) | (1 << ROTACAO_ANTIHORARIA_PIN));
    PORTB |= ((1 << FRENTE_PIN) | (1 << TRAS_PIN) | (1 << ESQUERDA_PIN)| (1 << DIREITA_PIN) | (1 << ROTACAO_HORARIA_PIN) | (1 << ROTACAO_ANTIHORARIA_PIN));

    DDRD |= (1 << MOTOR3_PIN);
    setup_pwm_timer0();
    config_encoder();

    while (1) {
        float vx = 0;
        float vy = 0;
        float omega = 0;

        if (!(PIND & (1 << FRENTE_PIN))) {
            vx = 1; // Velocidade para frente
        } else if (!(PIND & (1 << TRAS_PIN))) {
            vx = -1; // Velocidade para trás
        }

        if (!(PIND & (1 << ESQUERDA_PIN))) {
            vy = 1; // Velocidade para esquerda
        } else if (!(PIND & (1 << DIREITA_PIN))) {
            vy = -1; // Velocidade para direita
        }

        if (!(PINB & (1 << ROTACAO_HORARIA_PIN))) {
          omega = 1; //Rotaciona no sentido horario
        }else if (!(PINB & (1 << ROTACAO_ANTIHORARIA_PIN))){
          omega = -1; //Rotaciona no sentido anti-horario
        }

        float omega1 = (-vx + (2 * vy) / sqrt(3.0)) / R;
        float omega2 = (-vx - (2 * vy) / sqrt(3.0)) / R;
        float omega3 = (2 * vx) / R;

        // Converte as velocidades angulares para valores de PWM (0-255)
        uint8_t pwm1 = (uint8_t)(fabs(omega1) * (MAX_PWM / 10)); // Ajuste o fator de escala (10) conforme necessário
        uint8_t pwm2 = (uint8_t)(fabs(omega2) * (MAX_PWM / 10));
        uint8_t pwm3 = (uint8_t)(fabs(omega3) * (MAX_PWM / 10));

        set_motor_speed(pwm1, pwm2, pwm3);
    }
    return 0;
}