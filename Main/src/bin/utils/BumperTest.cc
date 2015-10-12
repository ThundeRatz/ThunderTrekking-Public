#include <stdexcept>
#include <iostream>
#include <ctime>

#include "Bumper.hh"
#include "Leds.hh"

using namespace std;
using namespace Trekking;

int main() {
	Bumper bumper;
	Leds led("LedBlue");

	cout << "Bumper\n";

	while(!(bumper.pressed()));

	for (;;) {
		if(bumper.pressed()) {
			led = 1;
			cout << "Estado: 1\n";
		}
		else {
			// led = 0;
		    cout << "Estado: 0\n";
		}
	}
	return 0;
}
