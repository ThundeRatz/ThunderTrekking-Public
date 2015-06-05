#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "udp_sender.h"
#include "leds.h"
#include "ports.h"

int main() {
	int udp_socket;
	uint8_t freq, duty, tempo, cor;
	
	if ((udp_socket = udp_sender_init(UDP_LEDS)) == -1) {
		perror("udp_receiver_init");
		return -1;
	}
	
	for (;;) {
		scanf("%hhu %hhu %hhu %hhu", &cor, &freq, &duty, &tempo);
		led_set(cor ,freq, duty, tempo);
	}
}
