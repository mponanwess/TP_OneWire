#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include <stdint.h>
#include "gpio.h"

/* Position de la ligne One-Wire : port et numero de broche */
struct ONEWIRE_Config {
	GPIO_TypeDef*	port;		// GPIOA, GPIOB, ...
	uint16_t		pin;		// 0 a 15
};

uint8_t ONEWIRE_Reset(struct ONEWIRE_Config* config);
void    ONEWIRE_WriteBit0(struct ONEWIRE_Config* config);
void    ONEWIRE_WriteBit1(struct ONEWIRE_Config* config);
uint8_t ONEWIRE_ReadBit(struct ONEWIRE_Config* config);

#endif
