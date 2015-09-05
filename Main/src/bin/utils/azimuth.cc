#include <iostream>

#include "GPS.hh"

using namespace std;

int main() {
	Trekking::GPS from, to;

	for (;;) {
		cin >> from.latitude >> from.longitude >> to.latitude >> to.longitude;
		if (!cin.good())
			return 0;
		cout << from.azimuth_to(to) << "\n";
	}
}
