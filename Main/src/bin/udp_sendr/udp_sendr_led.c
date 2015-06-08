#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "udp_sender.h"
#include "leds.h"
#include "ports.h"

int main() {
	int udp_socket, tempo;
	uint8_t freq, duty, cor, data;
	
	if ((udp_socket = udp_sender_init(UDP_LEDS)) == -1) {
		perror("udp_receiver_init");
		return -1;
	}
	
	for (;;) {
		scanf("%hhu %hhu %hhu %d", &cor, &freq, &duty, &tempo);
		//led_set(cor ,freq, duty, tempo);
		data = 255;
		udp_sender_send(udp_socket, &data, sizeof(data));
	}
}
