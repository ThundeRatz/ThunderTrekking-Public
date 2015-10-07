#include "Leds.hh"
using namespace Trekking;
int main() {
	Leds led;
	led.setTimestep(10);
	led.red(255);
	led.green(255);
	led.blue(255);
	led.setMode(BLINK);
	led.red(255);
	led.green(255);
	led.blue(255);
	led.setMode(PULSE3);
	led.red(0);
	led.green(0);
	led.blue(0);
	led.setTimestep(1);
	led.setTimestep(255);
	led.setMode(BLINK);
	return 0;
}

