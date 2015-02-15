#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "adc.h"
#include "spi.h"
#include "ui.h"

int main() {

	
	// input pin @ PB0 detects channel lock with push switch (so activate pull-up)
	PORTB |= _BV(PB0);
	
	// initialize SPI
	setup_spi();
	
	// setup PWM outputs
	setup_pwm_outputs();
	
	// start AD conversions
	adc_setup();
    run_adc_timer();
	
    // Enable interrupts for all. GO! =)
    SREG |= 0x80 ;
	
	while (1) {
		_delay_ms(500);
		// PORTB = PORTB ^ 0b00000011;
		
	}
	
	
}
