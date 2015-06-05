#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "udp_receiver.h"
#include "ports.h"

int main() {
	int udp_socket;
	struct sockaddr_in remote;
	char ip[41];
	double data[2], soma1 = 0, soma2 = 0;
	int n = 0;
	
	if ((udp_socket = udp_receiver_init(UDP_GPS_CELL, sizeof(data))) == -1) {
		perror("udp_sender_init");
		return -1;
	}
	
	for (;;) {
		switch (udp_receiver_recv(udp_socket, &data, sizeof(data))) {
			case sizeof(data):
			if (inet_ntop(AF_INET, &remote.sin_addr, ip, sizeof(ip) - 1) == NULL) {
				perror("inet_ntop");
				printf("%.8f %.8f\n", data[0], data[1]);
			} else
				printf("%s: %.8f %.8f\n", ip, data[0], data[1]);
				soma1 += data[0]; soma2 += data[1]; n++;
			break;
			
			case -1:
			perror("recvfrom");
			return -1;
			
			default:
			printf("Unexpected message size\n");
			break;
		}
		if (n == 14) {
			printf("Media: %.8f %.8f\n", soma1/n, soma2/n);
			n = soma1 = soma2 = 0;
		}
	}
}
