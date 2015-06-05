#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gpio_dma.h"
#include "thread_spawn.h"
#include "thread_bigodes.h"
#include "thread_hc_sr04.h"
#include "thread_hmc5883l.h"
#include "thread_leds.h"

int main() {
    gpio_dma_init();
    //thread_spawn(bigodes_thread,  NULL);
    thread_spawn(leds_thread,  NULL);
    thread_spawn(hc_sr04_thread,  NULL);
    thread_spawn(hmc5883l_thread, NULL);
    for (;;)
        pause();
    return 0;
}
