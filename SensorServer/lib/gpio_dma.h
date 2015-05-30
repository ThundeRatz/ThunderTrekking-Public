#ifndef __GPIO_DMA_H__
#define __GPIO_DMA_H__

#include <stdint.h>

static inline void gpio_dma_enable_i2c_id() {}
static inline void gpio_dma_input_with_pull(unsigned int pin, unsigned int pull_up_down) {}
void gpio_dma_init();
void gpio_dma_free();
void gpio_dma_set(unsigned int pin, unsigned int value);
uint8_t gpio_dma_get(unsigned int pin);

#endif
