#include <iostream>

#include "sleep.hh"
#include "GPIOButton.hh"

int main() {
	Trekking::GPIOButton pino(164);
	for(;;) {
		if(pino)
			printf("pino 164 ativo\n");
		else
			printf("pino 164 inativo\n");
		Trekking::sleep_ms(300);
	}
	return 0;
}
