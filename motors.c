#include "i2c.h"
#ifdef DEBUG
#include <stdio.h>
#endif

static int motors;

int motor_init() {
	motors = i2c_open(1, 0x69);
	return motors;
}

int motor_right(int value) {
	if (i2c_write(motors, 0, value < 0))
		return -1;
	if (i2c_write(motors, 1, value > 0 ? value : -value))
		return -1;
	return 0;
}

int motor_left(int value) {
	if (i2c_write(motors, 2, value < 0))
		return -1;
	if (i2c_write(motors, 3, value > 0 ? value : -value))
		return -1;
	return 0;
}

int motor(int right, int left) {
#ifdef DEBUG
	printf("motor - %d %d\n", right, left);
	if (right < -255 || right > 255 || left < -255 || left > 255)
		printf("INVALID MOTOR SPEED!\n");
#endif
#warning Remover spinloop aqui
	while (motor_right(right)) ;
	while (motor_left(left)) ;
/*
	if (motor_right(right))
		return -1;
	if (motor_left(left))
		return -1;
*/
	return 0;
}

void motor_free() {
	close(motors);
}
