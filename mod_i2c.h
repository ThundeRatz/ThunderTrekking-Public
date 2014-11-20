#ifndef __MOD_I2C_H__
#define __MOD_I2C_H__

#include <pthread.h>
#include <stdint.h>

enum {REG_DIR_D, REG_VEL_D, REG_DIR_E, REG_VEL_E,
	REG_R, REG_G, REG_B, REG_LOWADC, REG_HIGHADC, REG_MODE, REG_TIMESTEP,
	REG_COMPASS_CONF0, REG_COMPASS_CONF1, REG_COMPASS_CONF2, REG_COMPASS_X, REG_COMPASS_Y, REG_COMPASS_Z};

void mod_i2c_create();
void mod_i2c_write(int reg, uint8_t value);
void mod_i2c_write_force(int reg, uint8_t value);
uint8_t mod_i2c_read(int reg);
uint16_t mod_i2c_read_word(int reg);
#endif
