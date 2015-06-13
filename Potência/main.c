#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "init.h"
#include "radio_isr.h"
#include "usart.h"
#include "mixagem.h"
#include "watchdog.h"

#define DERIV_DELAY		4450
#define DEADZONE		1
#define WATCHDOG_MAX    75

#define STATUS_TOOGLE	PORTB ^= (1<<PB4)
#define STATUS_ON		PORTB |= (1<<PB4)
#define STATUS_OFF		PORTB &= ~(1<<PB4)

int __attribute__((noreturn)) main(void) {
	init();
	radio_start();
    for (;;) {
		static uint8_t revert_left = 0, revert_right = 0,
			speed, watchdog_counter = 0;
		static int8_t speed_left, speed_right;

        _delay_us(DERIV_DELAY);
		if (wdt_check_reset())
            watchdog_counter = 0;
        else
            if (watchdog_counter < WATCHDOG_MAX)
                watchdog_counter++;

		if (channel_3 > 110) {
			uint16_t mixado;
			STATUS_ON;
			USART_Stop();
			wdt_pass(WDT_USART1);
			wdt_pass(WDT_USART2);

			mixado = mixagem(channel_1, channel_2);
			// mixagem
			speed_left = (mixado >> 8) / 2;
			speed_right = (mixado & 0xffff) / 2;
		} else {
			STATUS_OFF;
			USART_Start();
			if (channel_2 > 50) {
				speed_left = usart_channel_1;
				speed_right = usart_channel_2;
			} else {
				speed_left = speed_right = 0;
			}
		}

		if (watchdog_counter < WATCHDOG_MAX && (speed_left < -DEADZONE || speed_left > DEADZONE ||
			speed_right < -DEADZONE || speed_right > DEADZONE))
            speed = 4 * (speed_left < 0 ? -speed_left : speed_left);
        else
            speed = 0;

        if ((speed_left < 0) == revert_left) {
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
				revert_left = (speed_left < 0);
        }

		if (watchdog_counter < WATCHDOG_MAX && (speed_left < -DEADZONE || speed_left > DEADZONE ||
			speed_right < -DEADZONE || speed_right > DEADZONE))
            speed = 4 * (speed_right < 0 ? -speed_right : speed_right);
        else
            speed = 0;

		if (speed < 10)
			speed = 0;
        else if (speed > 250)
			speed = 250;

        if ((speed_right < 0) == revert_right) {
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
				revert_right = speed_right < 0;
        }
    }
}
