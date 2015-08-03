#include <iostream>
#include "GPIO.hh"

using namespace Trekking;
using namespace std;

int main() {
	GPIO bumper(21);

	bumper.direction(GPIO::IN);
	bumper.edge(GPIO::EDGE_BOTH);
	for (;;) {
		bumper.poll();
		cout << (bumper ? "ON" : "OFF") << endl;
	}
}
