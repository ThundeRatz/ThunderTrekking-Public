#ifndef __LEDS_H__
#define __LEDS_H__

#define LEDS2000MS 56
#define LEDS1750MS 48
#define LEDS1500MS 40
#define LEDS1250MS 32
#define LEDS1000MS 24
#define LEDS750MS 16
#define LEDS500MS 8
#define LEDS250MS 0

#define LEDS100PC 192
#define LEDS75PC 128
#define LEDS50PC 64
#define LEDS25PC 0

#define TODOS 7
#define LED_R 1
#define LED_G 2
#define NENHUM 0

void led_set(uint8_t cor, uint8_t frequencia, uint8_t duty, int tempo);
void leds_init();

#endif