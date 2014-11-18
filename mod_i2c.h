#include <pthreads.h>

enum {REG_DIR_D, REG_VEL_D, REG_DIR_E, REG_VEL_E, REG_R, REG_G, REG_B, REG_LOWADC, REG_HIGHADC, REG_MODE, REG_TIMESTEP};

void mod_i2c_create();
void mod_i2c_write(int reg, uint8_t value);
