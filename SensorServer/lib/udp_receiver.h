#ifndef __UDP_RECEIVER_H__
#define __UDP_RECEIVER_H__

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>

int udp_receiver_init(uint16_t port);

static inline ssize_t udp_receiver_recv(int udp_socket, void *data, size_t length) {
	return recv(udp_socket, data, length, 0);
}

#endif

