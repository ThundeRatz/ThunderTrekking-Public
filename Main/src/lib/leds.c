#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "udp_sender.h"
#include "leds.h"
#include "ports.h"
#include "thread_spawn.h"

#define MS	1000000

static uint8_t data;
static int udp_socket;
static struct timespec time2;

void leds_init()
{
    udp_socket = udp_sender_init(UDP_LEDS);
}

void led_set(uint8_t cor, uint8_t frequencia, uint8_t duty, int tempo)
{
    if (tempo < 1000) {
        time2.tv_sec = 0;
        time2.tv_nsec = tempo * MS;
    } else {
        time2.tv_sec = tempo/1000;
        time2.tv_nsec = 0;
    }
    data = cor + duty + frequencia;
    udp_sender_send(udp_socket, &data, sizeof(data));
}
