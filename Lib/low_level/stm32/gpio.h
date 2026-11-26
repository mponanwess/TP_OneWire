#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include <stm32f446xx.h>

/* Modes d'une broche (registre MODER, 2 bits par broche) */
#define INPUT_MODE 		0
#define OUTPUT_MODE		1
#define ALT_MODE 		2
#define ANALOG_MODE		3

/* Masques pour effacer 1 a 5 bits */
#define BITMASK_1BITS	0b1
#define BITMASK_2BITS	0b11
#define BITMASK_3BITS	0b111
#define BITMASK_4BITS	0b1111
#define BITMASK_5BITS	0b11111

void 		GPIO_Init(void);
void 		GPIO_SetPin(GPIO_TypeDef* GPIO, uint16_t pin);
void 		GPIO_ResetPin(GPIO_TypeDef* GPIO, uint16_t pin);
void 		GPIO_TogglePin(GPIO_TypeDef* GPIO, uint16_t pin);
uint16_t 	GPIO_ReadPin(GPIO_TypeDef* GPIO, uint16_t pin);

#endif
