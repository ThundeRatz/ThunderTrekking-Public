#include "LedsI2C.hh"

namespace Trekking {
	enum {
		MANUAL, BLINK, PULSE, PULSE3
	};

    enum {
		COR, LOWADC, HIGHADC, MODE, TIMESTEP
	};

	void Leds::red(unsigned value) {
		i2c.acquire(0x24);

		i2c.release();
	}
}
