#ifndef __GPIO_DMA_H__
#define __GPIO_DMA_H__

#define GPIO_DMA_PULLDOWN		1
#define GPIO_DMA_PULLUP			2

extern volatile unsigned *gpio_dma_map;

void gpio_dma_init();
void gpio_dma_free();
void gpio_dma_input_with_pull(unsigned int pin, unsigned int pull_up_down);
void gpio_dma_enable_i2c_id();

static inline void gpio_dma_input(unsigned int pin) {
	*(gpio_dma_map + ((pin) / 10)) &= ~(7 << (((pin) % 10) * 3));
}

static inline void gpio_dma_output(unsigned int pin) {
	gpio_dma_input(pin);
	*(gpio_dma_map + ((pin) / 10)) |=  (1 << (((pin) % 10) * 3));
}

static inline void gpio_dma_alt(unsigned int pin, unsigned int alternate) {
	gpio_dma_input(pin);
	*(gpio_dma_map + (((pin) / 10))) = (*(gpio_dma_map + (((pin) / 10))) & ~(7 << (pin % 10) * 3)) | (alternate << (pin % 10) * 3);
}

static inline void gpio_dma_set(unsigned int pin) {
	*(gpio_dma_map + 7) = pin;
}

static inline void gpio_dma_clear(unsigned int pin) {
	*(gpio_dma_map + 10) = pin;
}

static inline unsigned int gpio_dma_get(unsigned int pin) {
	return (*(gpio_dma_map + 13) & (1 << pin));
}

#endif
