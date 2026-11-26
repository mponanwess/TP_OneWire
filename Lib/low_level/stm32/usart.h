#ifndef USART_H_
#define USART_H_

#include <stdint.h>

void USART2_Init(void);
void USART2_TransmitChar(uint8_t data);
uint8_t USART2_ReceiveChar(void);

#endif
