#include "hook.h"

void mov_update();

void mov_init() {
	hook_install(HOOK_MOV, mov_update);
}


double mov_delta(int perimetro, double ang_inicial, double ang_final) {
	// Δs = r * Θ => r = Δs / Θ
	double ray, delta, alpha = ang_inicial - ang_final;
	
	if (alpha == 0.)
		return perimetro;
	
	ray = perimetro / alpha;
	return 2 * ray * sin(alpha / 2);
}
