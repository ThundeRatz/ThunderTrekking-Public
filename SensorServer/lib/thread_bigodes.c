#define BIGODE_R    26
#define BUMPER      25

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "gpio_dma.h"
#include "udp_sender.h"
#include "ports.h" //teste

#define MS	1000000

void __attribute__((noreturn)) *bigodes_thread(__attribute__((unused)) void *ignored) {
	const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 50 * MS};
    int8_t status_antigo = 0, status = 0;
    int socket = udp_sender_init(UDP_BIGODE);

    if (socket == -1) {
        perror("udp_sender_init");
        exit(-1);
    }

    gpio_dma_input_with_pull(BIGODE_L, GPIO_DMA_PULLUP);
    gpio_dma_input_with_pull(BIGODE_R, GPIO_DMA_PULLUP);
    gpio_dma_input_with_pull(BUMPER,   GPIO_DMA_PULLUP);

    for (;;) {
        status = (!gpio_dma_get(BIGODE_L)) | ((!gpio_dma_get(BIGODE_R)) << 1) | ((!gpio_dma_get(BUMPER)) << 2);
        if (status != status_antigo) {
            status_antigo = status;
            printf("%hhu\n", status);
            if (udp_sender_send(socket, &status, 1) == -1)
                perror("udp_sender_send");
        }

        if (nanosleep(&sleep_time, NULL) == -1)
            perror("nanosleep");
    }
}
