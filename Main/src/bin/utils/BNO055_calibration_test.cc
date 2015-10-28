#include <eigen3/Eigen/Dense>
#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "BNO055.hh"
#include "sleep.hh"

using namespace std;

int main() {
	cout << fixed;
	Trekking::BNO055 bno055;

	for(;;) {
		bno055.get_calibration();
		Trekking::sleep_ms(100);
	}

	return 0;
}
