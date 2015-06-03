#pragma once

#include <avr/wdt.h>

enum {
	WDT_CH1, WDT_CH2, WDT_CH3,
	WDT_END
};

extern uint8_t watchdog_status;

void wdt_check_reset();

static inline void wdt_pass(uint8_t id) {
	watchdog_status |= 1 << id;
}
