#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
	int udp_socket;
	struct sockaddr_in local, remote;
	float data[3];
	char ip[41];
	socklen_t addr_len = sizeof(struct sockaddr_in);
	uint16_t listen_port;
	if (argc < 2) {
		fprintf(stderr, "Uso:\n\t%s listen_port\n", argv[0]);
		return -1;
	}
	
	if (sscanf(argv[1], "%hu", &listen_port) != 1) {
		fprintf(stderr, "sscanf falhou\n");
		return -1;
	}
	
	if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return -1;
	}
	
	memset((char *) &local, 0, sizeof(local));
	local.sin_family = AF_INET;	//IPv4
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(listen_port);
	
	if (bind(udp_socket, (struct sockaddr *) &local, sizeof(local)) == -1) {
		perror("bind");
		return -1;
	}
	
	while (recvfrom(udp_socket, data, sizeof(data), 0, (struct sockaddr *) &remote, &addr_len) != -1) {
		if (inet_ntop(AF_INET, &remote.sin_addr, ip, sizeof(ip) - 1) == NULL) {
			perror("inet_ntop");
			printf("%f %f %f\n", data[0], data[1], data[2]);
		} else {
			printf("%s: %f %f %f\n", ip, data[0], data[1], data[2]);
		}
	}
	
	perror("recvfrom");
	return 0;
}
