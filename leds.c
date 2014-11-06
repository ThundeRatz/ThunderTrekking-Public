#include "i2c.h"

// Código rápido só pra ligar os leds quando o trekking iniciar.
// Será reescrito para um módulo decente de leds

void setup_leds() {
	int fd = i2c_open(1, 0x24);
	
	if (fd != -1) {
		i2c_write(fd, 5, 3);
		i2c_write(fd, 6, 1);
	}
}
