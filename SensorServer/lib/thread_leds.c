#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include "gpio_dma.h"
#include "udp_receiver.h"
#include "ports.h"

// Portas dos LEDs
#define LED_R   26
#define LED_G   19
#define LED_B   6 // Non eexiste

#define MS	1000000

void __attribute__((noreturn)) *leds_thread(__attribute__((unused)) void *ignored) {
    const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 50 * MS};
    int socket = udp_receiver_init_nonblock(UDP_LEDS);

    int8_t led[5]; //led[0] = Red; led[1] = Green; led[2] = Blue; led[3] = Periodo; led[4] = "Duty"
    int per, duty1, duty2;
    uint8_t pacote = 0, pacotec = 0, pacote_antigo = 0;

    if (socket == -1) {
        perror("udp_receiver_init");
        exit(-1);
    }

    for (;;) {
         if (udp_receiver_recv(socket, &pacote, sizeof(pacote)) == -1) {
			const struct timespec sleep_error = {.tv_sec = 0, .tv_nsec = 500 * MS};
			if (errno != EAGAIN && errno != EWOULDBLOCK) {
			    perror("udp_receiver_recv");
                nanosleep(&sleep_error, NULL);
    			continue;
            }
		}
        if (pacote != pacote_antigo) {     //Se o status muda, ver qual o status novo e realizar a acao
            pacotec = pacote;
            for (int i = 0; i < 3; i++) {
                led[i] = pacotec%2;        // Salva os valores para os LEDs
                pacotec /= 2;
            }
            led[3] = pacotec%8;  // Salva o valor do periodo
            pacotec /= 8;
            led[4] = pacotec%4;  // Salva o valor do Duty Cicle

            per = (led[3] + 1)*125;          // Periodo em MS
            duty1 = (per * (led[4] + 1))/4;  // Tempo aceso
            duty2 = (per * (3 - led[4]))/4;  // Tempo apagado
            struct timespec periodo1;
            struct timespec periodo2;
            if (duty1 >= 1000) {
                periodo1.tv_sec = 1; periodo1.tv_nsec = 0;
            } else {
                periodo1.tv_sec = 0; periodo1.tv_nsec = duty1 * MS;
            }
            if (duty2 >= 1000) {
                periodo2.tv_sec = 1; periodo2.tv_nsec = 0;
            } else {
                periodo2.tv_sec = 0; periodo2.tv_nsec = duty2 * MS;
            }

            pacote_antigo = pacote;
            while (pacote == pacote_antigo) {
                gpio_dma_set(LED_R, led[0]);
                gpio_dma_set(LED_G, led[1]);
                gpio_dma_set(LED_B, led[2]);
                printf("RGB: %d %d %d\n", led[0], led[1], led[2]);
                if (nanosleep(&periodo1, NULL))
                    perror("leds - nanosleep");
                printf("Tempo: %d\n", duty1);
                gpio_dma_set(LED_R, 0);
                gpio_dma_set(LED_G, 0);
                gpio_dma_set(LED_B, 0);
                if (nanosleep(&periodo2, NULL))
                    perror("leds - nanosleep");
                printf("RGB: %d %d %d\n", 0, 0, 0);
                printf("Tempo: %d\n\n", duty2);

                if (udp_receiver_recv(socket, &pacote, sizeof(pacote)) == -1) {
        			const struct timespec sleep_error = {.tv_sec = 0, .tv_nsec = 500 * MS};
        			if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        perror("udp_receiver_recv");
                        nanosleep(&sleep_error, NULL);
                    }
        			continue;
        		}
            }
        }

        if (nanosleep(&sleep_time, NULL) == -1)
            perror("nanosleep");
    }
}
