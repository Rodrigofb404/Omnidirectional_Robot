#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Variáveis globais para o motor
volatile uint16_t pulsos = 0;       // Contador de pulsos do encoder
volatile float rpm = 0;             // Velocidade em RPM do motor
float rpm_ideal = 1000;             // Velocidade desejada
float kp = 0.25, ki = 0.05, kd = 0.01;
float erro = 0, erro_anterior = 0;
float p = 0, i = 0, d = 0;
float pid = 0;

// Configuração do timer para medir RPM
void config_timer1() {
    TCCR1B = (1 << WGM12) | (1 << CS12);  // CTC mode, prescaler de 256
    OCR1A = 62500;  // 1 segundo (16 MHz / 256 / 62500 = 1 Hz)
    TIMSK1 = (1 << OCIE1A);  // Habilitar interrupção de comparação
}

// Configuração do PWM para o motor
void config_pwm() {
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1); // Fast PWM, não-invertido
    TCCR0B = (1 << CS01);  // Prescaler de 8
    DDRD |= (1 << PD6);    // Configura PD6 como saída (OC0A)
}

// Interrupção para contar pulsos do encoder
ISR(INT0_vect) {
    pulsos++;  // Incrementa contador de pulsos
}

// Interrupção para cálculo de RPM
ISR(TIMER1_COMPA_vect) {
    rpm = (float)(pulsos * 60) / 20;  // Ajuste para o número de pulsos por rotação
    pulsos = 0;  // Zera o contador de pulsos após o cálculo
}

// Controle PID para o motor
void pid_control() {
    // Calcula o erro
    erro = rpm_ideal - rpm;

    // Termos do PID
    p = erro * kp;
    i += erro * ki;
    d = (erro - erro_anterior) * kd;

    // Calcular saída do PID
    pid = p + i + d;

    // Limitar saída entre 0 e 255
    if (pid < 0) pid = 0;
    if (pid > 255) pid = 255;

    // Atualizar PWM
    OCR0A = (uint8_t)pid;

    // Atualizar erro anterior
    erro_anterior = erro;
}

