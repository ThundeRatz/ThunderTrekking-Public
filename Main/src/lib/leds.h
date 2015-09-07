/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 ThundeRatz

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

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
