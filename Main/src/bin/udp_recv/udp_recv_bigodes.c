#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "udp_receiver.h"
#include "ports.h"

int main() {
	int udp_socket;
	struct sockaddr_in remote;
	char ip[41];
	uint8_t data;
	
	if ((udp_socket = udp_receiver_init(UDP_BIGODE, sizeof(data))) == -1) {
		perror("udp_sender_init");
		return -1;
	}
	
	for (;;) {
		switch (udp_receiver_recv(udp_socket, &data, sizeof(data))) {
			case sizeof(data):
			if (inet_ntop(AF_INET, &remote.sin_addr, ip, sizeof(ip) - 1) == NULL) {
				perror("inet_ntop");
				printf("%hhu\n", data);
			} else
				printf("%s: %hhu\n", ip, data);
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
