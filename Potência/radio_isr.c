#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "main.h"

#define MIN			128
#define MAX			255
#define MID_RANGE	64

volatile uint8_t channel_3;

void radio_start() {
	EIMSK |= (1 << INT0) | (1 << INT1);
}

void radio_stop() {
	EIMSK = 0;
}

ISR(INT0_vect) {
	if (PIND & (1 << PIND2)) {
		TCNT2 = 0;				// Zera o Timer 2
		TCCR2B |= (1 << CS22) | (1 << CS20);	// Inicia o Timer 2 (CLKio / 64)
		return;
	}
	wdt_reset();
	uint8_t reading = TCNT2;
	TCCR2B = 0;
	if (TIFR2 & (1 << TOV2)) {	// Overflow
		TIFR2 |= (1 << TOV2);
		channel_1 = MAX - MIN - MID_RANGE;
		return;
	}
#if MAX < 255
	if (reading > MAX) {
		channel_1 = MAX - MIN - MID_RANGE;
		return;
	}
#endif
#if MIN > 0
	if (reading < MIN) {
		channel_1 = -MID_RANGE + 1;
		return;
	}
#endif
	channel_1 = reading - MIN - MID_RANGE;
	if (channel_1 == -64)
		channel_1++;
}

ISR(INT1_vect) {
	if (PIND & (1 << PIND3)) {
		TCNT2 = 0;				// Zera o Timer 2
		TCCR2B |= (1 << CS22) | (1 << CS20);	// Inicia o Timer 2 (CLKio / 64)
		return;
	}
	uint8_t reading = TCNT2;
	TCCR2B = 0;
	if (TIFR2 & (1 << TOV2)) {	// Overflow
		TIFR2 |= (1 << TOV2);
		channel_2 = MAX - MIN - MID_RANGE;
		return;
	}
#if MAX < 255
	if (reading > MAX) {
		channel_2 = MAX - MIN - MID_RANGE;
		return;
	}
#endif
#if MIN > 0
	if (reading < MIN) {
		channel_2 = -MID_RANGE + 1;
		return;
	}
#endif
	channel_2 = reading - MIN - MID_RANGE;
	if (channel_2 == -64)
		channel_2++;
}

ISR(PCINT0_vect) {
	if (PINB & (1 << PINB5)) {
		TCNT2 = 0;
		TCCR2B |= (1 << CS22) | (1 << CS20);
		return;
	}
	uint8_t reading = TCNT2;
	TCCR2B = 0;
	if (TIFR2 & (1 << TOV2)) {
		TIFR2 |= (1 << TOV2);
		reading = MAX;
	}
	channel_3 = reading / 2;
}
