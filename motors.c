#include "mod_i2c.h"
#ifdef DEBUG
#include <stdio.h>
#endif

void motor_right(int value) {
	mod_i2c_write(REG_DIR_D, value < 0);
	mod_i2c_write(REG_VEL_D, value > 0 ? value : -value);
}

void motor_left(int value) {
	mod_i2c_write(REG_DIR_E, value < 0);
	mod_i2c_write(REG_VEL_E, value > 0 ? value : -value);
}

void motor(int right, int left) {
	motor_right(right);
	motor_left(left);
#ifdef DEBUG
	printf("motor - %d %d\n", right, left);
	if (right < -255 || right > 255 || left < -255 || left > 255)
		printf("INVALID MOTOR SPEED!\n");
#endif
}

void motor_init() {
	mod_i2c_write_force(REG_VEL_E, 0);
	mod_i2c_write_force(REG_VEL_D, 0);
}
