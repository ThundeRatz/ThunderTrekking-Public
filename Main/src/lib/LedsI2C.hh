#pragma once

#include "I2C.hh"

namespace Trekking {
	enum {
			MANUAL, BLINK, PULSE, PULSE3
		};

	enum {
			CORG, CORR, CORB, LOWADC, HIGHADC, MODE, TIMESTEP
		};

	class LedsI2C {
		private:
			I2C i2c{BNO055_I2C_BUS, BNO055_I2C_BUS_NAME};
		public:
			LedsI2C();
			void red(unsigned value);
			void green(unsigned value);
			void blue(unsigned value);
			void white(unsigned value);
			void apaga();

			void blink(unsigned timestep);
			void pulse(unsigned timestep);
			void pulse3(unsigned timestep);

			void setMode(unsigned value);
			void setTimestep(unsigned value);

	};
}
