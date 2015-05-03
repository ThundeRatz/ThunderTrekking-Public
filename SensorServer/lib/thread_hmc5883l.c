#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "hmc5883l.h"
#include "ports.h"
#include "udp_sender.h"

#define MS	1000000

void __attribute__((noreturn)) *hmc5883l_thread(__attribute__((unused)) void *ignored) {
    const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 12 * MS};	// ~75 Hz, para menos
    const struct timespec subsleep_time = {.tv_sec = 0, .tv_nsec = 4 * MS};
    int16_t *data;
    int socket = udp_sender_init(UDP_HMC5883L);

    if (socket == -1) {
	perror("udp_sender_init");
	exit(-1);
    }

    if (hmc5883l_init())
	exit(-1);

    for (;;) {
	nanosleep(&sleep_time, NULL);
	while ((data = hmc5883l_read()) == NULL)
	    nanosleep(&subsleep_time, NULL);
	if (udp_sender_send(socket, data, 6) == -1)
	    perror("hmc5883l - udp_sender_send");
    }
}
