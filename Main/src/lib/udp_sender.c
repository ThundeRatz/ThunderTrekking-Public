#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "ports.h"

int udp_sender_init(uint16_t port) {
	struct sockaddr_in remote;
	int udp_socket;
	if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return -1;
	}
	
	memset((char *) &remote, 0, sizeof(remote));
	remote.sin_family = AF_INET;	//IPv4
	if (!inet_pton(AF_INET, IP, &remote.sin_addr.s_addr)) {
		perror("inet_pton");
		return -1;
	}
	remote.sin_port = htons(port);

	if (connect(udp_socket, (struct sockaddr *) &remote, sizeof(remote)) == -1) {
		perror("connect");
		return -1;
	}
	
	return udp_socket;
}
