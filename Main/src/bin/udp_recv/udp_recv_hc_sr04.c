#include <stdio.h>
#include <arpa/inet.h>

#include "udp_receiver.h"
#include "ports.h"
#include "udp_packets.h"

int main(int argc, char **argv) {
	int udp_socket, show_only_id = -1;
	struct sockaddr_in remote;
	char ip[41];
	hc_sr04_udp_packet_t data;
	
	if (argc > 1)
		sscanf(argv[1], "%d", &show_only_id);
	
	if ((udp_socket = udp_receiver_init(UDP_HC_SR04, sizeof(data))) == -1) {
		perror("udp_sender_init");
		return -1;
	}
	
	for (;;) {
		switch (udp_receiver_recv(udp_socket, &data, sizeof(data))) {
			case sizeof(data):
			if (inet_ntop(AF_INET, &remote.sin_addr, ip, sizeof(ip) - 1) == NULL) {
				perror("inet_ntop");
				if (show_only_id < 0 || show_only_id == data.id)
					printf("%hhu: %llu\n", data.id, ((long long unsigned) data.distance) / 1000 / 58);
			} else {
				if (show_only_id < 0 || show_only_id == data.id)
					printf("%s: %hhu: %llu\n", ip, data.id, ((long long unsigned) data.distance) / 1000 / 58);
			}
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
