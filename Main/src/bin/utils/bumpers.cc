#include <iostream>
#include "GPIOButton.hh"

using namespace Trekking;
using namespace std;

int main() {
	GPIOButton bumper(21);

	for (;;)
		cout << (bumper ? "ON" : "OFF") << endl;
}
