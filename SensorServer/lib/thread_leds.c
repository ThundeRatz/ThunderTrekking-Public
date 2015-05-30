#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#inlcude <time.h>
#include <errno.h>
#include "gpio_dma.h"
#include "udp_receiver.h" 
#include "ports.h" 

#define LED_R    13   // Portas dos LEDs
#define LED_G    19   
#define LED_B    26   

#define APAG 0 // Definir os status possiveis pro LED
#define ACES 1       

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

    printf("Leds inicializados.\n");
    
    for (;;) {
        printf("Esperando dados...\n");
         if (udp_receiver_recv(socket, &pacote, sizeof(pacote)) == -1) {  // Colocar o Nonblock la
			const struct timespec sleep_error = {.tv_sec = 0, .tv_nsec = 500 * MS};
			perror("udp_receiver_recv");
			if (errno != EAGAIN && errno != EWOULDBLOCK)
                nanosleep(&sleep_error, NULL);
			continue;
		}
        printf("Pacote recebido: %hhu\n", pacote);
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
            const struct timespec periodo1 = {.tv_sec = 0, .tv_nsec = duty1 * MS};
            const struct timespec periodo2 = {.tv_sec = 0, .tv_nsec = duty2 * MS};
            
            pacote_antigo = pacote;
            while (pacote == pacote_antigo) {
                if (led[0] == ACES) {           // Acende os LEDs que devem ser acesos
                    gpio_dma_set(LED_R, ACES);
                    printf("LEDR aceso\n");
        		}
                if (led[1] == ACES) {
                    gpio_dma_set(LED_G, ACES);
                    printf("LEDG aceso\n"); 
        		}
                if (led[2] == ACES) {
                    gpio_dma_set(LED_B, ACES);
                    printf("LEDB aceso\n"); 
        		}          
                nanosleep(&periodo1, NULL);  
                gpio_dma_set(LED_R, APAG);      // Apaga todos os LEDs 
                printf("LEDR desligado\n");
                gpio_dma_set(LED_G, APAG);     
                printf("LEDG desligado\n");        		
                gpio_dma_set(LED_B, APAG);      
                printf("LEDB desligado\n");                           
                nanosleep(&periodo2, NULL); 
                
                if (udp_receiver_recv(socket, &pacote, sizeof(pacote)) == -1) {  // Colocar o Nonblock la
        			const struct timespec sleep_error = {.tv_sec = 0, .tv_nsec = 500 * MS};
        			perror("udp_receiver_recv");
        			if (errno != EAGAIN && errno != EWOULDBLOCK)
                        nanosleep(&sleep_error, NULL);
        			continue;
        		}
            }            
        }
        if (nanosleep(&sleep_time, NULL) == -1)
            perror("nanosleep");
    }
}
