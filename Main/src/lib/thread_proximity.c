#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#include "thread_spawn.h"
#include "udp_receiver.h"
#include "ports.h"

float sensor_contato = 999;

void __attribute__((noreturn)) *proximity_thread(__attribute__((unused)) void *ignored) {
	int udp_socket;
	udp_socket = udp_receiver_init(UDP_PROXIMITY, sizeof(sensor_contato));
	if (udp_socket == -1) {
		perror("udp_receiver_init");
		exit(-1);
	}
	
	for (;;) {
		switch (udp_receiver_recv(udp_socket, &sensor_contato, sizeof(sensor_contato))) {
			case sizeof(sensor_contato):
			break;

			case -1:
			perror("recvfrom");
			break;

			default:
			printf("Unexpected message size\n");
			break;
		}
	}
}
