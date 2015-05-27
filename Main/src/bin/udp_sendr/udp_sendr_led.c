#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "udp_sender.h"
#include "ports.h"

int main() {
	int udp_socket;
	uint8_t data;
	
	if ((udp_socket = udp_sender_init(UDP_LEDS) == -1) {
		perror("udp_receiver_init");
		return -1;
	}
	
	for (;;) {
		scanf("%hhu", &data);
		udp_sender_send(udp_socket, &data, sizeof(data));
	
	}
}
