#ifndef __LEDS_H__
#define __LEDS_H__

#include "mod_i2c.h"

enum leds_modes {
	MANUAL, BLINK, PULSE, PULSE3
};

enum leds_reg {
	COLOR, R = 0, G, B, LOWADC, HIGHADC, MODE, TIMESTEP
};

#define leds_color(r, g, b)		do {mod_i2c_write(REG_R, r); mod_i2c_write(REG_G, g); mod_i2c_write(REG_B, b);} while (0)
#define leds_mode(mode)			mod_i2c_write(REG_MODE, mode)
#define leds_timestep(ts)		mod_i2c_write(REG_TIMESTEP, ts)

#warning TODO ADC

#endif
