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

	cout << "Bumper\n";

	while(!(bumper.pressed()));

	for (;;) {
		if(bumper.pressed()) {
			cout << "Estado: 1\n";
			if (ledOn)
				led = 0;
			else
				led = 1;
			ledOn = !ledOn;
		}
		else {
			// led = 0;
		    //cout << "Estado: 0\n";
		}
	}
	return 0;
}
