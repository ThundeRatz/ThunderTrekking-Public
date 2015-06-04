#pragma once

#include <stdint.h>
#include <avr/wdt.h>

enum {
	WDT_CH1, WDT_CH2, WDT_CH3,
	WDT_END
};

extern uint8_t __watchdog_status;

uint8_t wdt_check_reset();

static inline void wdt_pass(uint8_t id) {
	__watchdog_status |= 1 << id;
}
