#include <stdio.h>

#include "thread_spawn.h"
#include "thread_motors.h"

int main() {
	thread_spawn(motors_thread, NULL);

	for (;;) {
		int left, right;
		scanf("%d%d", &left, &right);
		motor(left, right);
	}
	return 0;
}
