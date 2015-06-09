#pragma once

#include <stdint.h>

extern volatile int8_t channel_1, channel_2;
extern volatile uint8_t channel_3;

void radio_start();
void radio_stop();
