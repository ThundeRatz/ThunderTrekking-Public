#ifndef __HC_SR04_UDP_H__
#define __HC_SR04_UDP_H__

#include <stdint.h>

typedef struct {
	uint8_t id;
	uint64_t distance;
} hc_sr04_udp_packet_t;

int hc_sr04_udp_init();
int hc_sr04_recv(hc_sr04_udp_packet_t *data);

#endif

