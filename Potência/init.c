#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "usart.h"

void init(void) {
    cli();
    MCUSR &= ~(1<<WDRF);	// Limpa flags do WDT

	DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2);
    DDRC = (1 << PC1);
    DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
    PORTB |= (1 << PB0);
    PORTC |= (1 << PC1);
    PORTD |= (1 << PD7) | (1 << PD4);

    wdt_reset();
    WDTCSR = (1 << WDCE) | (1 << WDIE) | (1 << WDP1) | (1 << WDP0);

    // Interrupção INT0/1 em subida e descida
    EICRA |= (1 << ISC00) | (1 << ISC10);
    // PCIE0 em PB5 (SCK, PCINT5)
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT5);

	// Inicializa o contador 0 para gerar PWM
	TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);	// PWM phase correct com 8 bits
	TCCR0B = (1 << CS01);									// Preescaler em clk/8
    OCR0A = 0;
    OCR0B = 0;

	// Inicializa o contador 1 para gerar PWM
	TCCR1A = (1<<COM1A1) | (1<<COM1B1) | (1<<WGM10);	// PWM phase correct com 8 bits
	TCCR1B = (1 << CS11);								// Preescaler em clk/8
    OCR1A = 0;
    OCR1B = 0;

	USART_Init();
    sei();
}
