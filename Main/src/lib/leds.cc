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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "udp_sender.h"
#include "leds.h"
#include "ports.h"
#include "ThreadSpawn.hh"

#define MS	1000000

static uint8_t data;
static int udp_socket;
static struct timespec time2;

void leds_init()
{
    udp_socket = udp_sender_init(UDP_LEDS);
}

void led_set(uint8_t cor, uint8_t frequencia, uint8_t duty, int tempo)
{
    if (tempo < 1000) {
        time2.tv_sec = 0;
        time2.tv_nsec = tempo * MS;
    } else {
        time2.tv_sec = tempo/1000;
        time2.tv_nsec = 0;
    }
    data = cor + duty + frequencia;
    udp_sender_send(udp_socket, &data, sizeof(data));
}
