#pragma once
#include "I2C.hh"
namespace Trekking {

	enum {
			MANUAL, BLINK, PULSE, PULSE3
		};

	enum {
			CORG, CORR, CORB, LOWADC, HIGHADC, MODE, TIMESTEP
		};

	class Leds {
		private:
			I2C i2c{1, "i915 gmbus vga"};
		public:
	 	   	Leds();
		       ~Leds();		
			void red(unsigned value);
			void green(unsigned value);
			void blue(unsigned value);
			void white(unsigned value);
			void apagaLeds();

			void blink(unsigned timestep);
			void pulse(unsigned timestep);
			void pulse3(unsigned timestep);
	
			void setMode(unsigned value);
			void setTimestep(unsigned value);
		
	};
}

