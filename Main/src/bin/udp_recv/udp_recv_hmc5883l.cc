#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "udp_receiver.h"
#include "ports.h"
#include "compass.h"

int main() {
	int udp_socket;
	struct sockaddr_in remote;
	char ip[41];
	int16_t data[3];
	
	udp_socket = udp_receiver_init(UDP_HMC5883L, sizeof(data));
	if (udp_socket == -1) {
		perror("udp_sender_init");
		return -1;
	}
	
	for (;;) {
		switch (udp_receiver_recv(udp_socket, &data, sizeof(data))) {
			case sizeof(data):
			if (inet_ntop(AF_INET, &remote.sin_addr, ip, sizeof(ip) - 1) == NULL) {
				perror("inet_ntop");
				printf("%hd %hd %hd %lf\n", data[0], data[1], data[2], compass_orientation(data[0], data[2]));
			} else
				printf("%s: %hd %hd %hd %lf\n", ip, data[0], data[1], data[2], compass_orientation(data[0], data[2]));
			break;
			
			case -1:
			perror("udp_receiver_recv");
			return -1;
			
			default:
			printf("Unexpected message size\n");
			break;
		}
	}
}
