// user interface LED drivers 

#include <avr/io.h>

void setup_pwm_outputs() {
	// output pins
	DDRB |= _BV(PB2);
	DDRA |= _BV(PA7);
	
    // setup for fast PWM
    TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00);
    TCCR0B = (1<<CS00);

	// also set PB1 as output to drive RED LED.
	DDRB |= _BV(PB1);

}

void setleds(int8_t key,int8_t tr,int8_t channel) {
	uint8_t intensity=((uint8_t)key*3);
	if ( tr == 0 || tr == 12 || tr == -12 ) {
		intensity = intensity+100;
	}
	if (channel == 1) { // ch 1
		OCR0A = intensity;
	} 
	if (channel == 2) {  // ch 2.
		OCR0B = intensity;
	}
}
