#include "mod_i2c.h"
#include "compass.h"
#include <unistd.h>
#include <math.h>
/*
void compass_init() {
}
*/

void compass_read(struct compass *c) {
	// Configurações copiadas do programa em python, buscar especificações depois
	// e ver o que pode ser configurado
	mod_i2c_write_force(REG_COMPASS_CONF0, 0x70);	// 8 samples @ 15Hz
	mod_i2c_write_force(REG_COMPASS_CONF1, 0x20);	// 1.3 gain LSb / Gauss 1090 (default)
	mod_i2c_write_force(REG_COMPASS_CONF2, 0x00);	// continuous sampling
	
#warning SYNC aqui
	c->x = (int16_t) mod_i2c_read_word(REG_COMPASS_X);
	c->y = (int16_t) mod_i2c_read_word(REG_COMPASS_Y);
	c->z = (int16_t) mod_i2c_read_word(REG_COMPASS_Z);
}

double compass_orientation() {
	struct compass c;
	compass_read(&c);
	
	return atan2((((double) c.y)), (((double) c.x)));
}

double compass_diff(double target, double now) {
	double r;
	r = fmod((target - now), 2 * M_PI);
	return (r < M_PI ? r : r - 2 * M_PI);
}
