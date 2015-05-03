#ifndef __UDP_PACKETS__
#define __UDP_PACKETS__

#include <stdint.h>

typedef struct {
	uint8_t id;
	uint64_t distance;
} hc_sr04_udp_packet_t;

#endif
