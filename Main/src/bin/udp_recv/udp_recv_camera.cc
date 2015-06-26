#include <stdio.h>
#include <stdint.h>

#include "udp_receiver.h"
#include "ports.h"
#include "camera.h"

int main() {
	int udp_socket;
	camera_data data;
	
	if ((udp_socket = udp_receiver_init(UDP_CAMERA, sizeof(camera_data))) == -1) {
		perror("udp_sender_init");
		return -1;
	}
	
	for (;;) {
		switch (udp_receiver_recv(udp_socket, &data, sizeof(data))) {
			case sizeof(data):
			printf("%f %f %f %f\n", data.top_left.x, data.top_left.y,
				data.bottom_right.x, data.bottom_right.y);
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
