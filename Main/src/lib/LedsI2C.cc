#include "LedsI2C.hh"
namespace Trekking {

	LedsI2C::LedsI2C() {
		i2c.acquire(0x24);
		i2c[MODE] = MANUAL;
		i2c[CORR] = 0;
		i2c[CORG] = 0;
		i2c[CORB] = 0;
		i2c.release();
	}


	void LedsI2C::setMode(unsigned value) {
		i2c.acquire(0x24);
		i2c[MODE] = value;
		i2c.release();
	}

	void LedsI2C::setTimestep(unsigned value) {
		i2c.acquire(0x24);
		i2c[TIMESTEP] = value;
		i2c.release();
	}


	void LedsI2C::red(unsigned value) {
		i2c.acquire(0x24);
		i2c[CORR] = value;
		i2c.release();
	}

	void LedsI2C::green(unsigned value) {
		i2c.acquire(0x24);
		i2c[CORG] = value;
		i2c.release();
	}

	void LedsI2C::blue(unsigned value) {
		i2c.acquire(0x24);
		i2c[CORB] = value;
		i2c.release();
	}

	void LedsI2C::white(unsigned value) {
		i2c.acquire(0x24);
		i2c[CORR] = value;
		i2c[CORG] = value;
		i2c[CORB] = value;
		i2c.release();
	}

	void LedsI2C::apaga() {
		i2c.acquire(0x24);
		i2c[CORR] = 0;
		i2c[CORG] = 0;
		i2c[CORB] = 0;
		i2c.release();
	}
	void LedsI2C::blink(unsigned timestep) {
		i2c.acquire(0x24);
		setMode(BLINK);
		setTimestep(timestep);
		i2c.release();
	}

	void LedsI2C::pulse(unsigned timestep) {
		i2c.acquire(0x24);
		setMode(PULSE);
		setTimestep(timestep);
		i2c.release();
	}

	void LedsI2C::pulse3(unsigned timestep) {
		i2c.acquire(0x24);
		setMode(PULSE3);
		setTimestep(timestep);
		i2c.release();
	}
}
