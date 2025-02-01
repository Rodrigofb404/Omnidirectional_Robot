#include <atmega328p.h>
#define AVR8_STUB_DISABLE_INTERRUPTS
#include <avr8-stub.h>
// #include "avr8-stub.h"

volatile uint64_t counter1 = 0;
volatile uint64_t counter2 = 0;
volatile uint64_t counter3 = 0;
volatile uint16_t rpm_motor1 = 0;
volatile uint16_t rpm_motor2 = 0;
volatile uint16_t rpm_motor3 = 0;

// ======================================================================
// Maybe it's not necessary
// 0 - ClockWise
// 1 - Counter clockWise
// ======================================================================
volatile int8_t motor1_direction; 
volatile int8_t motor2_direction;
volatile int8_t motor3_direction;


// Counter pulses for
ISR(PCINT0_vect) {
	int8_t Q1_rise_edge = (PINB & (1 << PB0)) ? 1 : 0; 
	int8_t Q2_high = (PINB & (1 << PB1)) ? 1 : 0;

	if (Q1_rise_edge && Q2_high) {
		// Clockwise
		counter3++;
		// motor3_direction = 0; 
	} else if (Q1_rise_edge && !Q2_high) {
		// Counter Clockwise
		counter3--;
		// motor3_direction = 1; 
	}
}

ISR(PCINT1_vect) {
	int8_t Q1_rise_edge = (PINC& (1 << PC0)) ? 1 : 0; 
	int8_t Q2_high = (PINC & (1 << PC1)) ? 1 : 0;

	if (Q1_rise_edge && Q2_high) {
		// Clockwise
		counter1++;
		// motor1_direction = 0; 
	} else if (Q1_rise_edge && !Q2_high) {
		// Counter Clockwise
		counter1--;
		// motor1_direction = 1; 
	} 
}

ISR(PCINT2_vect) {
	int8_t Q1_rise_edge = (PINC& (1 << PC0)) ? 1 : 0; 
	int8_t Q2_high = (PINC & (1 << PC1)) ? 1 : 0;

	if (Q1_rise_edge && Q2_high) {
		// Clockwise
		counter2++;
		// motor2_direction = 0; 
	} else if (Q1_rise_edge && !Q2_high) {
		// Counter Clockwise
		counter2--;
		// motor2_direction = 1; 
	} 
}

ISR(TIMER1_COMPA_vect) {
	// breakpoint();
	rpm_motor1 = rpm_calc(counter1, 20);
	rpm_motor2 = rpm_calc(counter2, 20);
	rpm_motor3 = rpm_calc(counter3, 20);

	// reset the pulses count for next measurement
	counter1 = 0;
	counter2 = 0;
	counter3 = 0;

	PORTB ^= (1 << PB5);
}

// Função para inicializar a UART (Serial)
void UART_init(uint16_t baud_rate) {
    UBRR0H = (uint8_t)(baud_rate >> 8);
    UBRR0L = (uint8_t)baud_rate;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0); // Habilita TX e RX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Modo 8 bits, sem paridade
}


int main (void) {
	UART_init(103);
	IO_init();
	
	// config_timer0_PWM(0, 0, 4, 200, 0);
	config_timer2_PWM(0, 0, 2 , 200, 100);
	encoder(0);

	motor1_rotation(0);
	motor2_rotation(0);
	motor3_rotation(1);

	while (1)
	{
		printf("RPM Motor1: %u\n", rpm_motor1);
		printf("RPM Motor2: %u\n", rpm_motor2);
		printf("RPM Motor3: %u\n", rpm_motor3);
    _delay_ms(1000); // Atualiza a cada 1 segundo
	} 
}
