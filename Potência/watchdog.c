#include <avr/io.h>
#include <avr/interrupt.h>

#include "watchdog.h"
#include "main.h"

uint8_t __watchdog_status, watchdog_ok;

void wdt_check_reset() {
	if (__watchdog_status == (1 << WDT_END) - 1) {
		__watchdog_status = 0;
		wdt_reset();
		watchdog_ok = 1;
	}
}

ISR(WDT_vect){
    channel_2 = 0;
    channel_1 = 0;
	watchdog_ok = 0;
}
