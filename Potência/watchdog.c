#include "watchdog.h"

uint8_t watchdog_status;

void wdt_check_reset() {
	if (watchdog_status == (1 << WDT_END) - 1) {
		watchdog_status = 0;
		wdt_reset();
	}
}
