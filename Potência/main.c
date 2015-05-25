#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "init.h"
#include "radio_isr.h"
#include "usart.h"

#define ACEL			1314

#define STATUS_TOOGLE	PORTB ^= (1<<PB4)
#define STATUS_ON		PORTB |= (1<<PB4)
#define STATUS_OFF		PORTB &= ~(1<<PB4)

volatile int8_t channel_1, channel_2;

int map(int formerValue, int formerMin, int formerMax, int newMin, int newMax)
{
	int newValue;

	newValue = (formerValue - formerMin) * (newMax - newMin);
	newValue /= (formerMax - formerMin);
	newValue += newMin;

	return newValue;
}

int __attribute__((noreturn)) main(void) {
	init();
    for (;;) {
		static uint8_t current_direction_left = 0, current_direction_right = 0,
			speed;
		static int8_t speed_left, speed_right;

        _delay_us(ACEL);

		if (1) {//(channel_3 > 0)
			USART_Stop();
			radio_start();
			// mixagem
			speed_left = channel_1;
			speed_right = channel_2;
		} else {
			radio_stop();
			USART_Start();
			speed_left = channel_1;
			speed_right = channel_2;
		}


		speed = 2 * (speed_left < 0 ? -speed_left : speed_left);
		if (speed < 10)
			speed = 0;
        else if (speed > 250)
			speed = 250;

        if ((speed_left < 0) == current_direction_left) {
            if (speed_left < 0) {
				OCR0A = 0;
                if (OCR0B < speed)
                    OCR0B++;
                else if (OCR0B > speed)
                    OCR0B--;
            } else {
				OCR0B = 0;
                if (OCR0A < speed)
                    OCR0A++;
                else if (OCR0A > speed)
                    OCR0A--;
            }
        } else {
            if (OCR0B > 0)
                OCR0B--;
            if (OCR0A > 0)
                OCR0A--;
            if (!OCR0A && !OCR0B)
				current_direction_left = (speed_left < 0);
        }

		speed = 2 * (speed_right < 0 ? -speed_right : speed_right);
		if (speed < 10)
			speed = 0;
        else if (speed > 250)
			speed = 250;

        if ((speed_right < 0) == current_direction_right) {
            if (speed_right < 0) {
				OCR1A = 0;
                if (OCR1B < speed)
                    OCR1B++;
                else if (OCR1B > speed)
                    OCR1B--;
            } else {
				OCR1B = 0;
                if (OCR1A < speed)
                    OCR1A++;
                else if (OCR1A > speed)
                    OCR1A--;
            }
        } else {
            if (OCR1B > 0)
                OCR1B--;
            if (OCR1A > 0)
                OCR1A--;
            if (!OCR1A && !OCR1B)
				current_direction_right = speed_right < 0;
        }
    }
}
