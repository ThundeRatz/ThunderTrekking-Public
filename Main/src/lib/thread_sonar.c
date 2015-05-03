#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "udp_receiver.h"
#include "ports.h"
#include "udp_packets.h"

uint64_t sonar_l = 200, sonar_r = 200;

void __attribute__((noreturn)) *sonar_thread(__attribute__((unused)) void *ignored) {
	int udp_socket;
	hc_sr04_udp_packet_t data;
	
	if ((udp_socket = udp_receiver_init(UDP_HC_SR04, sizeof(data))) == -1) {
		perror("hmc5883l_thread - udp_sender_init");
		exit(-1);
	}
	
	for (;;) {
		switch (udp_receiver_recv(udp_socket, &data, sizeof(data))) {
			case sizeof(data):
			if (data.id == 0)
				sonar_l = data.distance;
			else
				sonar_r = data.distance;
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
