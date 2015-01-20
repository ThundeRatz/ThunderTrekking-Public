#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

static int udp_socket;

int hmc5883l_udp_init() {
	struct sockaddr_in local;
	
	if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return -1;
	}
	
	memset((char *) &local, 0, sizeof(local));
	local.sin_family = AF_INET;	//IPv4
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(14140);
	
	if (bind(udp_socket, (struct sockaddr *) &local, sizeof(local)) == -1) {
		perror("bind");
		return -1;
	}
	
	return 0;
}

int hmc5883l_udp_recv(int16_t *data) {
	switch (recv(udp_socket, data, 3 * sizeof(int16_t), 0)) {
		case -1:
		perror("hmc5883l_udp_recv - recv");
		return -1;
		
		case 3 * sizeof(int16_t):
		return 0;
		
		default:
		fprintf(stderr, "compass_udp_recv: Tamanho errado de mensagem\n");
		return -1;
	}
}

