#include <atmega328p.h>
#define AVR8_STUB_DISABLE_INTERRUPTS
// #include <avr8-stub.h>
#include "avr8-stub.h"

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


// Count pulses for MOTOR3
ISR(PCINT0_vect) {		
    if (PINB & (1 << PB0)) {
        // counter3 += PINB & (1 << PB1) ? 1 : -1;
    }
}


// Count pulses for MOTOR1
// Count pulses for MOTOR1
ISR(PCINT1_vect) { 
    if (PINC & (1 << PC0)) {
        counter1 += PINC & (1 << PC1) ? 1 : -1;
    }
}


// Count pulses for MOTOR2
ISR(PCINT2_vect) {
	if (PINC & (1 << PC0)) {
		counter2 += PINC & (1 << PC1) ? 1 : -1;	
	}
}

ISR(TIMER1_COMPA_vect) {
	rpm_motor1 = rpm_calc(counter1, 50);
	rpm_motor2 = rpm_calc(counter2, 50);
	rpm_motor3 = rpm_calc(counter3, 50);
	
	counter1 = 0;
	counter2 = 0;
	counter3 = 0;
}


int main (void) {
	IO_init();
	config_timer0_PWM(0, 0, 4, 255, 0);
	config_timer2_PWM(0, 0, 4 , 255, 255);
	encoder(0);

	motor1_rotation(0);
	motor2_rotation(0);
	motor3_rotation(0);

	while (1)
	{
		if (PINB & (1 << PB1)) {
			speed_up();
		} else if (PINB & (1 << PB2)) {
			speed_down();
		}
		
	} 
}
