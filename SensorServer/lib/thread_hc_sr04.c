#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "udp_sender.h"
#include "ports.h"
#include "udp_packets.h"

void __attribute__((noreturn)) *hc_sr04_thread(__attribute__((unused)) void *ignored) {
	static struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 100 * 1000000};
	int sonar0, sonar1, udp_socket;
	hc_sr04_udp_packet_t packet;
	
	sonar0 = open("/dev/hc-sr04-0", O_RDONLY);
	if (sonar0 == -1) {
		perror("hc-sr04 - open");
		exit(-1);
	}
	
	sonar1 = open("/dev/hc-sr04-1", O_RDONLY);
	if (sonar1 == -1) {
		perror("hc-sr04 - open");
		exit(-1);
	}
	
	if ((udp_socket = udp_sender_init(UDP_HC_SR04)) == -1) {
		perror("udp_sender_init");
		exit(-1);
	}
	
	for (;;) {
		if (read(sonar0, &packet.distance, sizeof(packet.distance)) < 0)
			perror("HR-SR04 - read");
		else {
			packet.id = 0;
			if (packet.distance < 10 * 1000 * 58)
				packet.distance = 0;
			else {
				packet.distance -= 10 * 1000 * 58;
				if (packet.distance > 600 * 1000 * 58)
					packet.distance = 600 * 1000 * 58;
			}
			if (udp_sender_send(udp_socket, &packet, sizeof(packet)) == -1)
				perror("udp_sender_send");
		}
		nanosleep(&sleep_time, NULL);
		
		if (read(sonar1, &packet.distance, sizeof(packet.distance)) < 0)
			perror("HR-SR04 - read");
		else {
			packet.id = 1;
			if (packet.distance < 10 * 1000 * 58)
				packet.distance = 0;
			else {
				packet.distance -= 10 * 1000 * 58;
				if (packet.distance > 600 * 1000 * 58)
					packet.distance = 600 * 1000 * 58;
			}
			if (udp_sender_send(udp_socket, &packet, sizeof(packet)) == -1)
				perror("udp_sender_send");
		}
		nanosleep(&sleep_time, NULL);
	}
}
