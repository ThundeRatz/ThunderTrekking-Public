#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "watchdog.h"
#include "main.h"

#define RIGHT		(1 << 7)
#define REVERSE		(1 << 6)
#define DATA_MASK	0x3f

void USART_Init() {
	UCSR0A = (1 << U2X0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);	// Modo Assincrono, Sem Paridade, 1 Stop-Bit, 8 Bits de dados
    UBRR0H = 0;
    UBRR0L = 207;	// Transmissão UART em ubrr=F_CPU/8/baud-1
}

void USART_Start() {
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0); //|(1 << TXEN0);	// Receiver, Transmiter e Interrupção por RX ativados
}

void USART_Stop() {
	UCSR0B = 0;
}

inline void USART_Transmit_Byte(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0)));	// Aguarda o termino de qualquer transmissão passada
    UDR0 = data;						// Coloca os dados no buffer, o que inicia automaticamente a transmissão
}

void USART_Transmit(unsigned char *data, unsigned int length) {
    unsigned int i;
    for (i = 0; i < length; i++)
		USART_Transmit_Byte(data[i]);
}

inline uint8_t USART_Receive(void) {
    while (!(UCSR0A & (1 << RXC0)));	// Aguarda dados serem recebidos
    return UDR0;						// Retorna o valor presente no buffer
}

ISR(USART_RX_vect) {
	static uint8_t new_data, old_data = 0;
	new_data = USART_Receive();
#pragma GCC diagnostic ignored "-Wsign-compare"
	if (new_data != ~old_data)
#pragma GCC diagnostic pop
		return;
	if (old_data & RIGHT) {
		wdt_pass(WDT_CH1);
		if (old_data & REVERSE)
			channel_1 = -(old_data & DATA_MASK);
		else
			channel_1 = old_data & DATA_MASK;
	} else {
		wdt_pass(WDT_CH2);
		if (old_data & REVERSE)
			channel_2 = -(old_data & DATA_MASK);
		else
			channel_2 = old_data & DATA_MASK;
	}
}
