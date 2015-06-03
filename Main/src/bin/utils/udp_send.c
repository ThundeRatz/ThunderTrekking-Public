#define u_short unsigned short

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
	int udp_socket;
	struct sockaddr_in remote;
	char data[200];
	size_t len;
	socklen_t addr_len = sizeof(struct sockaddr_in);
	uint16_t target_port;
	if (argc < 3) {
		fprintf(stderr, "Uso:\n\t%s remote_ip remote_port\n", argv[0]);
		return -1;
	}
	
	if (sscanf(argv[2], "%hu", &target_port) != 1) {
		fprintf(stderr, "sscanf falhou\n");
		return -1;
	}
	
	if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return -1;
	}
	
	memset((char *) &remote, 0, sizeof(remote));
	remote.sin_family = AF_INET;	//IPv4
	if (!inet_pton(AF_INET, argv[1], &remote.sin_addr.s_addr)) {
		perror("inet_pton");
		return -1;
	}
	remote.sin_port = htons(target_port);
	
	if (fgets(data, sizeof(data), stdin) == NULL) {
		perror("fgets");
		return -1;
	}
	len = strlen(data);
	if (data[len] == '\n') {
		data[len] = 0;
		len--;
	}
	if (sendto(udp_socket, data, len - 1, 0, (struct sockaddr *) &remote, addr_len) == -1)
		perror("sendto");
	return 0;
}
