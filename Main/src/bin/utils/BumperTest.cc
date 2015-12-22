#include <stdexcept>
#include <iostream>
#include <ctime>

#include "Bumper.hh"
#include "Leds.hh"

using namespace std;
using namespace Trekking;

int main() {
	Bumper bumper;
	Leds led("BNOBlue");
	bool ledOn = false;
	int code;

	cout << "Bumper\n";

	while(!(bumper.pressed()));

	for (;;) {
		if((code = bumper.pressed())) {
			cout << "Codigo: " << code << "  Estado: 1\n";
			led = ledOn ? 0 : 1;
			ledOn = !ledOn;
		}
		else {
			// led = 0;
		    //cout << "Estado: 0\n";
		}
	}
	return 0;
}
