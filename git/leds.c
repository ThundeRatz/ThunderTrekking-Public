/*
#include <stdint.h>
#include "mod_i2c.h"
#include "leds.h"

void leds_set(uint8_t mode, uint8_t reg) {
	
	self.bus.write_i2c_block_data(self.device, self.COR, [self.r, self.g, self.b])
	if mode != None:
		self.saved_mode = mode
	if timestep != None:
		self.saved_timestep = timestep
	self.bus.write_byte_data(self.device, self.MODE, self.saved_mode)
	self.bus.write_byte_data(self.device, self.TIMESTEP, self.saved_timestep)
}
*/
