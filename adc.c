
// ADC setup & data collection

#include <avr/io.h>
#include <avr/interrupt.h>

#include "spi.h"
#include "ui.h"

// ADC latest values
struct adc_data {
        int8_t cv1_in, cv1_transp, cv2_in, cv2_transp ;
} adc ;


void adc_setup() {
    // enable ADC, clock prescaler = /64
    ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0) ;

    // left adjust result
    ADCSRB |= (1<<ADLAR);

    // disable digital IO on used ADC pins
    DIDR0 = 0b00001111;

    // Go! =)
    ADCSRA |= (1<<ADSC) ;
}

void run_adc_timer() {  // enable 16bit timer1 + interrupts

	OCR1AH = 0x07; // 8MHz, divider 2000 -> 4KHz timer -> update CVs @ 1000Hz
	OCR1AL = 0xd0;

    // OCR1AH = 0x03; // 8MHz, divider 1000 -> 8KHz timer
    // OCR1AL = 0xe8;

    // clear on OCR1A, clock prescaler 1.
    TCCR1B |= (1<<WGM12) | (1<<CS10) ;
    // TCCR1B |= (1<<WGM12) | (1<<CS10) | (1<<CS11) ;  // presc. /64

    // enable OCR1A interrupt
    TIMSK1 |= (1<<OCIE1A);
}

int8_t tr2key(uint8_t tr) {
	// convert transpose knob value to key +/-
	int8_t key=-13+(tr/10);
	if ( key <= -1 ) {
		key++;
	}
	return(key);
}
 
int8_t cv2key(uint8_t cv) {
	// convert CV input value (0-255) -> key number 0-49
	int8_t key=cv/5;
	if ( key > 49 ) {
		key = 49;
	}
	return(key);
}

uint16_t key2dacv(int8_t key) {
	// convert key number to DAC output value/voltage in mV
	return(((uint16_t)key*1000)/12);
}


int8_t process_channel_key(int8_t cvkey, int8_t trkey, uint8_t channel) {
	int8_t retkey=cvkey + trkey;
	PORTB &= ~_BV(PB1); // clear RED led
	if ( retkey < 0 ) {  // hit bottom? Return 0 and light RED!
		retkey = 0;
		PORTB |= _BV(PB1);
	}
	if ( retkey > 49 ) {  // above top value? Cut and light RED!
		retkey = 49;
		PORTB |= _BV(PB1);
	}
	return(retkey);
}

ISR(TIM1_COMPA_vect) {

    static uint8_t adc_chan=0;
    uint8_t chan_mask=0x03; // use 4 channels (0-3).
    uint8_t adc_in = ADCH;  // read input value & invert it.

	// read channel lock switch (pressed = 0)
	uint8_t locksw = PINB & _BV(PB0);
	
    switch (adc_chan) {
        case 0 :        // chan 0, CV1 input
           	adc.cv1_in = cv2key(255 - adc_in);
			spi_write_word_cha(key2dacv(process_channel_key(adc.cv1_in,adc.cv1_transp,1)));
			setleds(adc.cv1_in,adc.cv1_transp,1);
           	break;
        case 2 :        // chan 2, CV1 transpose knob
            adc.cv1_transp = tr2key(adc_in);
            break;
        case 1 :        // chan 1, CV2 input
            adc.cv2_in = cv2key(255 - adc_in);
			if (!locksw) { // if switch is pressed (locksw = 0), copy cv1 -> cv2.
				adc.cv2_in = adc.cv1_in;
			}
			spi_write_word_chb(key2dacv(process_channel_key(adc.cv2_in,adc.cv2_transp,2)));
			setleds(adc.cv2_in,adc.cv2_transp,2);
            break;
        case 3 :        // chan 3, CV2transpose knob
            adc.cv2_transp = tr2key(adc_in);
            break;
    }

    adc_chan = ((adc_chan+1) & chan_mask ) ;

    // update ADMUX register
    ADMUX &= 0xf0;  // reset channel bits
    ADMUX |= adc_chan ;

    // start next conversion
    ADCSRA |= (1<<ADSC) ;
	
}