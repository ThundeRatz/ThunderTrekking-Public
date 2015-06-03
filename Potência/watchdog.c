#include <avr/io.h>
#include <avr/interrupt.h>

#include "watchdog.h"

uint8_t __watchdog_status, watchdog_ok;

void wdt_check_reset() {
	if (__watchdog_status == (1 << WDT_END) - 1) {
		__watchdog_status = 0;
		wdt_reset();
		watchdog_ok = 1;
	}
}

ISR(WDT_vect){
	watchdog_ok = 0;
}
