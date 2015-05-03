#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "udp_receiver.h"
#include "ports.h"

int main() {
	int udp_socket;
	struct sockaddr_in remote;
	char ip[41];
	float data[3];
	
	if ((udp_socket = udp_receiver_init(UDP_BUSSOLA_CELL, sizeof(data))) == -1) {
		perror("udp_sender_init");
		return -1;
	}
	
	for (;;) {
		switch (udp_receiver_recv(udp_socket, &data, sizeof(data))) {
			case sizeof(data):
			if (inet_ntop(AF_INET, &remote.sin_addr, ip, sizeof(ip) - 1) == NULL) {
				perror("inet_ntop");
				printf("%f %f %f\n", data[0], data[1], data[2]);
			} else
				printf("%s: %f %f %f\n", ip, data[0], data[1], data[2]);
			break;
			
			case -1:
			perror("recvfrom");
			return -1;
			
			default:
			printf("Unexpected message size\n");
			break;
		}
	}
}
