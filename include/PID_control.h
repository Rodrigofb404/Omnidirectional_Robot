#include <avr/io.h>
#include <avr/interrupt.h>


// Variáveis globais
extern volatile uint64_t counter0; // Contador global de pulsos do encoder (já configurado)
volatile float rpm = 0;            // Velocidade em RPM

// Declaração da função atualiza_pwm_motor
void atualiza_pwm_motor(uint8_t valor);
float rpm_ideal = 1000;            // Velocidade desejada
float kp = 0.25, ki = 0.05, kd = 0.01;
float erro = 0, erro_anterior = 0;
float p = 0, i = 0, d = 0;
float pid = 0;

// Configuração do Timer1 (para cálculo periódico de RPM)
void config_timer1() {
    TCCR1B = (1 << WGM12) | (1 << CS12);  // Modo CTC, prescaler 256
    TIMSK1 = (1 << OCIE1A);               // Habilitar interrupção de comparação
}

// Interrupção do Timer1: Cálculo de RPM
ISR(TIMER1_COMPA_vect) {
    // Calcula a RPM usando o contador global do encoder
    rpm = (float)(counter0 * 60);

    // Reseta o contador global do encoder para próxima medição
    counter0 = 0;
}

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

    // Limita a saída entre 0 e 255
    if (pid < 0) pid = 0;
    if (pid > 255) pid = 255;

    // Atualiza o PWM (função já configurada para o timer correspondente ao motor)
    atualiza_pwm_motor((uint8_t)pid);

    // Salva o erro atual para o próximo cálculo
    erro_anterior = erro;
}
