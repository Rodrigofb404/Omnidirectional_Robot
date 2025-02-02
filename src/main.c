#include <atmega328p.h>

// ======================================================================
// Counts the quantity of pulses comming from the encoders 
// ======================================================================
volatile int16_t counter1 = 0;
volatile int16_t counter2 = 0;
volatile int16_t counter3 = 0;


// ======================================================================
// Measures the RPM of the motors
// ======================================================================
volatile float rpm_motor1 = 0;
volatile float rpm_motor2 = 0;
volatile float rpm_motor3 = 0;

// ======================================================================
// Verifys what button is pressed to choose the direction
// ======================================================================
volatile uint8_t FORWARD_btn;
volatile uint8_t LEFT_btn;
volatile uint8_t RIGHT_btn;
volatile uint8_t BACKWARD_btn;
volatile uint8_t CW_btn;
volatile uint8_t CCW_btn;

// ======================================================================
// Maybe it's not necessary
// 0 - ClockWise
// 1 - Counter clockWise
// ======================================================================
volatile int8_t motor1_direction; 
volatile int8_t motor2_direction;
volatile int8_t motor3_direction;

// ======================================================================
// Verifys the state of the pins to detect rise or falling edge
// ======================================================================
volatile uint8_t currentQ1M1;
volatile uint8_t currentQ2M1;
volatile uint8_t currentQ2M2;
volatile uint8_t currentQ2M3;

// ======================================================================
// PWM adjusted by PID control
// ======================================================================
volatile uint8_t PID_pwmM1;
volatile uint8_t PID_pwmM2;
volatile uint8_t PID_pwmM3;

// ======================================================================
// PWM adjusted by PID control
// ======================================================================
float desired_RPM_M1 = 0;
float desired_RPM_M2 = 0;
float desired_RPM_M3 = 0;


// ======================================================================
// Count Pulses and read the direction of MOTOR1
// ======================================================================
ISR(PCINT1_vect) {	
	currentQ1M1 = (PINC & (1 << PC0)) ? 1 : 0;
	currentQ2M1 = (PINC & (1 << PC1)) ? 1 : 0;

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

// ======================================================================
// Count Pulses and read the direction of MOTOR2
// ======================================================================
ISR(INT1_vect) {
	currentQ2M2 = (PINC & (1 << PC2)) ? 1 : 0;
	
	if (currentQ2M2) {
		counter2++;
		motor2_direction = 0;
	} else if (!currentQ2M2) {
		counter2--;
		motor2_direction = 1;
	}
}

// ======================================================================
// Count Pulses and read the direction of MOTOR3
// ======================================================================
ISR(INT0_vect) {
	currentQ2M3 = (PINC & (1 << PC3)) ? 1 : 0;

	if (currentQ2M3) {
		counter3++;
		motor3_direction = 0;
	} else if (!currentQ2M3) {
		counter3--;
		motor3_direction = 1;
	}
}

// ======================================================================
// Call ISR each 30ms
// Calculate the RPM of the motor at each
// Do the polling of the push buttons to define the direction of the robot movement
// ======================================================================
ISR(TIMER1_COMPA_vect) {
	rpm_motor1 = rpm_calc(counter1, 300);
	rpm_motor2 = rpm_calc(counter2, 300);
	rpm_motor3 = rpm_calc(counter3, 300);

	FORWARD_btn = !(PIND & (1 << PD7)) ? 1 : 0;
	LEFT_btn = !(PINC & (1 << PC4)) ? 1 : 0; 
	RIGHT_btn = !(PINB & (1 << PB2)) ? 1 : 0;
	BACKWARD_btn = !(PINB & (1 << PB4)) ? 1 : 0;
	CW_btn = !(PINC & (1 << PC5)) ? 1 : 0;
	CCW_btn = 0;
	
	direction_control(FORWARD_btn, LEFT_btn, RIGHT_btn, BACKWARD_btn, CW_btn, CCW_btn, &desired_RPM_M1, &desired_RPM_M2, &desired_RPM_M3);

	PID_pwmM1 = pid_controlM1(rpm_motor1, fabs(desired_RPM_M1));
	OCR0A = PID_pwmM1;
	if (desired_RPM_M1 < 0) motor1_rotation(1);
	else motor1_rotation(0);

	PID_pwmM2 = pid_controlM2(rpm_motor2, fabs(desired_RPM_M2));
	OCR2A = PID_pwmM2;
	if (desired_RPM_M2 < 0) motor2_rotation(1);
	else motor2_rotation(0);

	PID_pwmM3 = pid_controlM3(rpm_motor3, fabs(desired_RPM_M3));
	OCR0B = PID_pwmM3;
	if (desired_RPM_M3 < 0) motor3_rotation(1);
	else motor3_rotation(0);

	counter1 = 0;
	counter2 = 0;
	counter3 = 0;
}


int main (void) {
	IO_init(); 						  // Initialize the IO's
	config_timer0_PWM(0, 0, 4, 0, 0); // Initialize TIMER0 PWM
	config_timer2_PWM(0, 0, 4, 0, 0); // Initialize TIMER2 PWM
	calc_coeficients_pid(); 		  // Calculate the PID coeficients
	encoder(0); 					  // Configure encoder

	// Define the start direction of the motors
	motor1_rotation(0);
	motor2_rotation(0);
	motor3_rotation(0);


// ======================================================================
// Define the direction of the robot movement based on what button is pressed
// Adjust the RPM for the desired value using PID
// ======================================================================
	while (1)
	{		
	} 
}
