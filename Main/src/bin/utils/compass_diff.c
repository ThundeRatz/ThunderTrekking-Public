#include <stdio.h>

#include "compass.h"

int main() {
	double target, current;
	
	while (scanf("%lf%lf", &target, &current) == 2)
		printf("Diff: %lf\n", compass_diff(target, current));
	return 0;
}

