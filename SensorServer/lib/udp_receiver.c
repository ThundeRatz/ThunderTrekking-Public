#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "ports.h"

int udp_receiver_init(uint16_t port) {
	struct sockaddr_in local;
	int udp_socket;
	if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return -1;
	}
	
	memset((char *) &local, 0, sizeof(local));
	local.sin_family = AF_INET;	//IPv4
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(port);

	if (bind(udp_socket, (struct sockaddr *) &local, sizeof(local)) == -1) {
		perror("bind");
		return -1;
	}
	
	return udp_socket;
}

