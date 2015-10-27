#include <eigen3/Eigen/Dense>
#include <iostream>
#include <cstdlib>
#include "BNO055.hh"
#include "sleep.hh"

int main() {
		Trekking::BNO055 bno055;
	
	for(;;)
		bno055.get_calibration();
	
	return 0;
	}
