#include <stdint.h>

#define ABS(x)		(x < 0 ? -x : x)
#define MAX(a, b)	(a > b ? a : b)
#define SIGN(x)		(x < 0 ? -1 : 1)

#define SPEED(x)	(x > 63 ? 63 : (x < -63 ? -63 : x))

uint16_t mixagem(int8_t x, int8_t y) {
	int8_t left, right, max_speed;

	//if (x < 10 && x > -10 && y < 10 && y > -10)
	//	return 0;

	max_speed = MAX(ABS(x), ABS(y));

	if ((x < 0) == (y < 0)) {
		left = max_speed * SIGN(x);
		right = max_speed * SIGN(x) * (ABS(y) - ABS(x)) / (ABS(y) + ABS(x));
	} else {
		left = max_speed * SIGN(x) * (ABS(y) - ABS(x)) / (ABS(y) + ABS(x));
		right = max_speed * SIGN(x);
	}

	// de [-63, 63]
	
	return (left << 8) | (right & 0xff);
}
