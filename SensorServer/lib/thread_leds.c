#define LED_R    XX   // Portas dos LEDs
#define LED_G    XX   
#define LED_B    XX   

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
    int socket = udp_receiver_init(UDP_LED);
    
    int8_t led[5]; //led[0] = Red; led[1] = Green; led[2] = Blue; led[3] = Periodo; led[4] = "Duty"
    int per, duty1, duty2;
    int8_t pacote;
    
    if (socket == -1) {
        perror("udp_receiver_init");
        exit(-1);
    }
    
    gpio_dma_output(LED_R);  // Seta as portasdos LEDs como saida
    gpio_dma_output(LED_G);  
    gpio_dma_output(LED_B);
  
    for (;;) {
        status = udp_receiver_recv(UDP_LEDS, &pacote, sizeof(pacote)) //Status vem de fora do programa
        statusc = pacote;
        if (status != status_antigo) {     //Se o status muda, ver qual o status novo e realizar a acao
            for (int i = 0; i < 3, i++) {
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
            
            // Se 0 => gpio_amd_clear(LED_X)
            // Se 1 => gpio_amd_set(LED_X)
            while (status == status_antigo) {
                if (led[0] == ACES)          // Acende os LEDs que devem ser acesos
                    gtio_dma_set(LED_R);
                if (led[1] == ACES)
                    gtio_dma_set(LED_G);
                if (led[2] == ACES)
                    gtio_dma_set(LED_B);
                nanosleep(&periodo1, NULL);  // Apaga todos os LEDs
                gtio_dma_clear(LED_R);
                gtio_dma_clear(LED_G);
                gtio_dma_clear(LED_B);
                nanosleep(&periodo2, NULL);
                status_antigo = status;
            }
        }
        
        if (nanosleep(&sleep_time, NULL) == -1)
            perror("nanosleep");
    }
}
