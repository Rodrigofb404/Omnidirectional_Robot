#include <atmega328p.h>

volatile int16_t counter1 = 0;
volatile int16_t counter2 = 0;
volatile int16_t counter3 = 0;
volatile int16_t rpm_motor1 = 0;
volatile int16_t rpm_motor2 = 0;
volatile int16_t rpm_motor3 = 0;

// ======================================================================
// Maybe it's not necessary
// 0 - ClockWise
// 1 - Counter clockWise
// ======================================================================
volatile int8_t motor1_direction; 
volatile int8_t motor2_direction;
volatile int8_t motor3_direction;


volatile uint8_t currentQ1M1;
volatile uint8_t currentQ2M1;
volatile uint8_t currentQ1M2;
volatile uint8_t currentQ2M2;
volatile uint8_t currentQ1M3;
volatile uint8_t currentQ2M3;
volatile uint8_t pwm1;
volatile uint8_t pwm2;
volatile uint8_t pwm3;

// Count pulses for MOTOR1
ISR(PCINT1_vect) {	
	currentQ1M1 = (PINC & (1 << PC0)) ? 1 : 0;
	currentQ2M1 = (PINC & (1 << PC1)) ? 1 : 0;
	// counter1++;
	if (currentQ1M1) {
		if (currentQ2M1) {
			counter1++;
			motor1_direction = 0;
		} else if (!currentQ2M1) {
			counter1--;
			motor1_direction = 1;
		}
	}
}

// Count pulses for MOTOR2
ISR(PCINT2_vect) {
	currentQ1M2 = (PIND & (1 << PD7)) ? 1 : 0;
	currentQ2M2 = (PINC & (1 << PC2)) ? 1 : 0;
	
	if (currentQ1M2) {
		if (currentQ2M2) {
			counter2++;
			motor2_direction = 0;
		} else if (!currentQ2M2) {
			counter2--;
			motor2_direction = 1;
		}
	}
}

// Count pulses for MOTOR3
ISR(PCINT0_vect) {
	currentQ1M3 = (PINB & (1 << PB0)) ? 1 : 0;
	currentQ2M3 = (PINC & (1 << PC3)) ? 1 : 0;

    if (currentQ1M3) {
		if (currentQ2M3) {
			counter3++;
			motor3_direction = 0;
		} else if (!currentQ2M3) {
			counter3--;
			motor3_direction = 1;
		}
	}
}

ISR(TIMER1_COMPA_vect) {
	rpm_motor1 = rpm_calc(counter1, 90);
	rpm_motor2 = rpm_calc(counter2, 90);
	rpm_motor3 = rpm_calc(counter3, 90);

	pwm1 = pid_control1(rpm_motor1, 50);
	OCR0A = pwm1;

	pwm2 = pid_control2(rpm_motor2, 90);
	OCR2A = pwm2;

	pwm3 = pid_control3(rpm_motor3, 150);
	OCR2B = pwm3;

	counter1 = 0;
	counter2 = 0;
	counter3 = 0;
}


int main (void) {
	IO_init();
	config_timer2_PWM(0, 0, 4, 200, 255);
	config_timer0_PWM(0, 0, 4, 220, 0);
	calc_coeficients_pid();
	encoder(0);

	motor1_rotation(0);
	motor2_rotation(0);
	motor3_rotation(0);

	while (1)
	{
		// if (!(PINB & (1 << PB1))) {
		// 	speed_up();
		// } 
		
		// if (!(PINB & (1 << PB2))) {
		// 	speed_down();
		// }	
	} 
}
