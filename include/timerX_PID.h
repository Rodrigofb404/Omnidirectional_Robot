#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>


// Variáveis globais para múltiplos motores
#define NUM_MOTORES 3
volatile uint16_t pulsos[NUM_MOTORES] = {0};  // Contador de pulsos para cada motor
volatile float rpm[NUM_MOTORES] = {0};        // Velocidade em RPM de cada motor
float rpm_ideal[NUM_MOTORES] = {1000, 1000, 1000};  // Velocidade desejada para cada motor
float kp[NUM_MOTORES] = {0.25, 0.25, 0.25};
float ki[NUM_MOTORES] = {0.05, 0.05, 0.05};
float kd[NUM_MOTORES] = {0.01, 0.01, 0.01};
float erro[NUM_MOTORES] = {0}, erro_anterior[NUM_MOTORES] = {0};
float p[NUM_MOTORES] = {0}, i[NUM_MOTORES] = {0}, d[NUM_MOTORES] = {0};
float pid[NUM_MOTORES] = {0};

// Configuração do timer para medir RPM
void config_timer1() {
    TCCR1B = (1 << WGM12) | (1 << CS12);  // CTC mode, prescaler de 256
    OCR1A = 62500;  // 1 segundo (16 MHz / 256 / 62500 = 1 Hz)
    TIMSK1 = (1 << OCIE1A);  // Habilitar interrupção de comparação
}

// Interrupção para contar pulsos do encoder
ISR(INT0_vect) {
    pulsos[0]++;  // Incrementa pulsos do motor 0
}

ISR(INT1_vect) {
    pulsos[1]++;  // Incrementa pulsos do motor 1
}

ISR(PCINT2_vect) {
    pulsos[2]++;  // Incrementa pulsos do motor 2
}

// Interrupção para cálculo de RPM
ISR(TIMER1_COMPA_vect) {
    for (int i = 0; i < NUM_MOTORES; i++) {
        rpm[i] = (float)(pulsos[i] * 60) / 20;  // Ajuste para o número de pulsos por rotação
        pulsos[i] = 0;
    }
}

// Controle PID para múltiplos motores
void pid_control() {
    for (int j = 0; j < NUM_MOTORES; j++) {
        erro[j] = rpm_ideal[j] - rpm[j];

        // Termos do PID
        p[j] = erro[j] * kp[j];
        i[j] += erro[j] * ki[j];
        d[j] = (erro[j] - erro_anterior[j]) * kd[j];

        // Calcular saída do PID
        pid[j] = p[j] + i[j] + d[j];

        // Limitar saída entre 0 e 255
        if (pid[j] < 0) pid[j] = 0;
        if (pid[j] > 255) pid[j] = 255;

        // Atualizar PWM do motor correspondente
        switch (j) {
            case 0: OCR0A = (uint8_t)pid[j]; break;
            case 1: OCR1A = (uint16_t)pid[j]; break;
            case 2: OCR2A = (uint8_t)pid[j]; break;
        }

        erro_anterior[j] = erro[j];
    }
}

