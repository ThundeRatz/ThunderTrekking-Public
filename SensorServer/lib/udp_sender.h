#ifndef __UDP_SENDER_H__
#define __UDP_SENDER_H__

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>

int udp_sender_init(uint16_t port);

static inline ssize_t udp_sender_send(int udp_socket, void *data, size_t length) {
	return send(udp_socket, data, length, 0);
}

#endif
