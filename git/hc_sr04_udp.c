#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#include "hc_sr04_udp.h"

static int udp_socket;

int hc_sr04_udp_init() {
	struct sockaddr_in local;
	int udp_buffer_size = 2 * sizeof(double);
	if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return -1;
	}
	
	setsockopt(udp_socket, SOL_SOCKET, SO_RCVBUF, &udp_buffer_size, sizeof(udp_buffer_size));
	
	memset((char *) &local, 0, sizeof(local));
	local.sin_family = AF_INET;	//IPv4
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(14141);
	
	if (bind(udp_socket, (struct sockaddr *) &local, sizeof(local)) == -1) {
		perror("bind");
		return -1;
	}
	
	return 0;
}

int hc_sr04_recv(hc_sr04_udp_packet_t *data) {
	switch (recv(udp_socket, data, sizeof(hc_sr04_udp_packet_t), 0)) {
		case -1:
		perror("recv");
		return -1;
		
		case sizeof(hc_sr04_udp_packet_t):
		return 0;
		
		default:
		fprintf(stderr, "gps_udp_recv: Tamanho errado de mensagem\n");
		return -1;
	}
}
