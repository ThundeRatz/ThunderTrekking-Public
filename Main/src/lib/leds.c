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

static void *led_set_thread(__attribute__((unused)) void *ignored) {
    udp_sender_send(udp_socket, &data, sizeof(data));
    if (nanosleep(&time2, NULL))
        perror("leds - nanosleep");
    data = NENHUM;
    udp_sender_send(udp_socket, &data, sizeof(data));
    return NULL;
}

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
    
    if (cor)
        cor = 7;
    data = cor + duty + frequencia;
    if (tempo == 0)
        udp_sender_send(udp_socket, &data, sizeof(data));
    else
        thread_spawn(led_set_thread, NULL);   
}
