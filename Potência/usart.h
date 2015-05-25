#pragma once

#include <stdint.h>

void USART_Init();
void USART_Start();
void USART_Stop();
inline void USART_Transmit_Byte(unsigned char data);
void USART_Transmit(unsigned char *data, int length);
inline uint8_t USART_Receive();
