#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SENSOR0_PIN PD2  // Pino do encoder do motor 0 (INT0)
#define PULSOS_POR_ROTACAO 20
#define FREQ_TIMER1 1     // Frequência de cálculo de RPM (1 Hz)

// Variáveis globais
volatile uint16_t pulsos = 0; // Contador de pulsos do encoder
volatile float rpm = 0;       // Velocidade em RPM
float rpm_ideal = 1000;       // Velocidade desejada
float kp = 0.25, ki = 0.05, kd = 0.01;
float erro = 0, erro_anterior = 0;
float p = 0, i = 0, d = 0;
float pid = 0;

// Configuração do Timer1 para cálculo de RPM
void config_timer1() {
    TCCR1B = (1 << WGM12) | (1 << CS12);  // Modo CTC, prescaler 256
    OCR1A = (F_CPU / 256) / FREQ_TIMER1;  // Configura para 1 Hz
    TIMSK1 = (1 << OCIE1A);               // Habilitar interrupção de comparação
}

// Interrupção para contagem de pulsos do encoder
ISR(INT0_vect) {
    pulsos++;  // Incrementa pulsos ao detectar borda descendente
}

// Interrupção do Timer1 para cálculo de RPM
// ISR(TIMER1_COMPA_vect) {
//     rpm = (float)(pulsos * 60) / PULSOS_POR_ROTACAO;  // Calcula RPM
//     pulsos = 0;  // Reseta contador de pulsos
// }

// Controle PID
void pid_control() {
    // Calcula erro
    erro = rpm_ideal - rpm;

    // Termos do PID
    p = kp * erro;
    i += ki * erro;
    d = kd * (erro - erro_anterior);

    // Saída do PID
    pid = p + i + d;

    // Limita saída entre 0 e 255
    if (pid < 0) pid = 0;
    if (pid > 255) pid = 255;

    // Atualiza PWM
    //timer0_PWM_value((uint8_t)pid);

    // Salva o erro atual para o próximo cálculo
    erro_anterior = erro;
}