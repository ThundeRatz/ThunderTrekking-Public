#include <stdio.h>

#include "ThreadSpawn.hh"
#include "ThreadMotors.hh"

int main() {
	thread_spawn(motors_thread);

	for (;;) {
		int left, right;
		scanf("%d%d", &left, &right);
		motor(left, right);
	}
	return 0;
}
