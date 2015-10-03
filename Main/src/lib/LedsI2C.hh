#pragma once

#include "I2C.hh"

namespace Trekking {
	class Leds {
	public:
		void red(unsigned value);
		void green(unsigned value);
		void blue(unsigned value);
		void blink(unsigned timestep);
		void pulse(unsigned timestep);
		void pulse3(unsigned timestep);

	private:
		I2C i2c{1, "i915 gmbus vga"};
	};
}
