#define LED_R    13   // Portas dos LEDs
#define LED_G    19
#define LED_B    26

#define APAG 0 // Definir os status possiveis pro LED
#define ACES 1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "gpio_dma.h"
#include "udp_receiver.h"
#include "ports.h"

#define MS	1000000

void __attribute__((noreturn)) *leds_thread(__attribute__((unused)) void *ignored) {
	const struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 50 * MS};
    int8_t status_antigo = 0, status = 0;
    int socket = udp_receiver_init(UDP_LEDS);

    int8_t led[5]; //led[0] = Red; led[1] = Green; led[2] = Blue; led[3] = Periodo; led[4] = "Duty"
    int per, duty1, duty2;
    uint8_t statusc, pacote;

    if (socket == -1) {
        perror("udp_receiver_init");
        exit(-1);
    }
    printf("Leds inicializados.\n");

    for (;;) {
		printf("Esperando dados...\n");
        if (udp_receiver_recv(socket, &pacote, sizeof(pacote)) == -1) {//Status vem de fora do programa
			const struct timespec sleep_error = {.tv_sec = 0, .tv_nsec = 500 * MS};
			perror("udp_receiver_recv");
			nanosleep(&sleep_error, NULL);
			continue;
		}
        statusc = pacote;
        printf("%hhu\n", pacote);
        if (statusc != status_antigo) {     //Se o status muda, ver qual o status novo e realizar a acao
            for (int i = 0; i < 3; i++) {
                led[i] = statusc%2;        // Salva os valores para os LEDs
                statusc /= 2;
            }
            led[3] = statusc%8;  // Salva o valor do periodo
            statusc /= 8;
            led[4] = statusc%4;  // Salva o valor do Duty Cicle

            per = (led[3] + 1)*125;          // Periodo em MS
            duty1 = (per * (led[4] + 1))/4;  // Tempo aceso
            duty2 = (per * (3 - led[4]))/4;  // Tempo apagado
            const struct timespec periodo1 = {.tv_sec = 0, .tv_nsec = duty1 * MS};
            const struct timespec periodo2 = {.tv_sec = 0, .tv_nsec = duty2 * MS};

            if (led[0] == ACES) {         // Acende os LEDs que devem ser acesos
				gpio_dma_set(LED_R, 1);
                printf("LEDR aceso\n");
			}
            else {
				gpio_dma_set(LED_R, 0);
                printf("LEDR apagado\n");
			}
            if (led[1] == ACES){
				gpio_dma_set(LED_G, 1);
			    printf("LEDG aceso\n");
			}
            else {
				gpio_dma_set(LED_G, 0);
                printf("LEDG apagado\n");
			}
            if (led[2] == ACES) {
				gpio_dma_set(LED_B, 1);
                printf("LEDB aceso\n");
			}
            else {
				gpio_dma_set(LED_B, 0);
				printf("LEDB apagado\n");
			}
            status_antigo = statusc;





            // Se 0 => gpio_amd_clear(LED_X)
            // Se 1 => gpio_amd_set(LED_X)
            /*while (status == status_antigo) {
                if (led[0] == ACES)          // Acende os LEDs que devem ser acesos
                    gpio_dma_set(LED_R);
                if (led[1] == ACES)
                    gpio_dma_set(LED_G);
                if (led[2] == ACES)
                    gpio_dma_set(LED_B);
                nanosleep(&periodo1, NULL);  // Apaga todos os LEDs
                gpio_dma_clear(LED_R);
                gpio_dma_clear(LED_G);
                gpio_dma_clear(LED_B);
                nanosleep(&periodo2, NULL);
                status_antigo = status;
            }*/
        }

        if (nanosleep(&sleep_time, NULL) == -1)
            perror("nanosleep");
    }
}
