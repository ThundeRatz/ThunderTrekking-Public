#include <avr/io.h>
#include <avr/interrupt.h>

#include "watchdog.h"

uint8_t __watchdog_status;

uint8_t wdt_check_reset() {
	if (__watchdog_status == (1 << WDT_END) - 1) {
		__watchdog_status = 0;
		wdt_reset();
		return 1;
	}
	return 0;
}
