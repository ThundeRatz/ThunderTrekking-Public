#ifndef __MOD_I2C_H__
#define __MOD_I2C_H__

#include <pthread.h>
#include <stdint.h>

enum {REG_DIR_D, REG_VEL_D, REG_DIR_E, REG_VEL_E,
	REG_R, REG_G, REG_B, REG_LOWADC, REG_HIGHADC, REG_MODE, REG_TIMESTEP,
	REG_HMC_CONFIGURATION_A, REG_HMC_CONFIGURATION_B, REG_HMC_MODE, REG_HMC_DATA_OUT_X_MSB,
	REG_HMC_DATA_OUT_X_LSB, REG_HMC_DATA_OUT_Z_MSB, REG_HMC_DATA_OUT_Z_LSB, REG_HMC_DATA_OUT_Y_MSB,
	REG_HMC_DATA_OUT_Y_LSB, REG_HMC_STATUS, REG_HMC_ID_A, REG_HMC_ID_B, REG_HMC_ID_C};

void mod_i2c_create();
void mod_i2c_write(int reg, uint8_t value);
void mod_i2c_write_now(int reg, uint8_t value);
void mod_i2c_write_force(int reg, uint8_t value);
uint8_t mod_i2c_read(int reg);
uint16_t mod_i2c_read_word(int reg);
uint16_t mod_i2c_read_word_inv(int reg);

#endif
