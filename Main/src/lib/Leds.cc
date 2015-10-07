#include "Leds.hh"
namespace Trekking {

	Leds::Leds() {
		i2c.acquire(0x24);
		i2c[MODE] = MANUAL;
		i2c[CORR] = 0;
		i2c[CORG] = 0;
		i2c[CORB] = 0;
		i2c.release();
	}

	Leds::~Leds(){
	}


	void Leds::setMode(unsigned value) {
		i2c.acquire(0x24);
		i2c[MODE] = value;
		i2c.release();
	}

	void Leds::setTimestep(unsigned value) {
		i2c.acquire(0x24);
		i2c[TIMESTEP] = value;
		i2c.release();
	}


	void Leds::red(unsigned value) {
		i2c.acquire(0x24);
		i2c[CORR] = value;
		i2c.release();
	}

	void Leds::green(unsigned value) {
		i2c.acquire(0x24);
		i2c[CORG] = value;
		i2c.release();
	}

	void Leds::blue(unsigned value) {
		i2c.acquire(0x24);
		i2c[CORB] = value;
		i2c.release();
	}

	void white(unsigned value) {
		i2c.acquire(0x24);
		i2c[CORR] = 255;
		i2c[CORG] = 255;
		i2c[CORB] = 255;
		i2c.release();
	}

	void Leds::apagaLeds() {
		i2c.acquire(0x24);
		i2c[CORR] = 0;
		i2c[CORG] = 0;
		i2c[CORB] = 0;
		i2c.release();
	}
	void Leds::blink(unsigned timestep) {
		i2c.acquire(0x24);
		setMode(BLINK);
		setTimestep(timestep);
		i2c.release();
	}

	void Leds::pulse(unsigned timestep) {
		i2c.acquire(0x24);
		setMode(PULSE);
		setTimestep(timestep);
		i2c.release();
	}

	void Leds::pulse3(unsigned timestep) {
		i2c.acquire(0x24);
		setMode(PULSE3);
		setTimestep(timestep);
		i2c.release();
	}
}

