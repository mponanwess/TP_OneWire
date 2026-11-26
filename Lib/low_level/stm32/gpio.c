#include "gpio.h"

void GPIO_Init(void){
	/* Clock Init */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

	/* GPIO Init */
	GPIO_SetMode(GPIOA, 5, OUTPUT_MODE);    // PA5  : LED LD2
	GPIO_SetMode(GPIOC, 13, INPUT_MODE);    // PC13 : bouton B1

	/* PA10 (D2) : ligne One-Wire. L'ordre compte ! */
	GPIO_SetPin(GPIOA, 10);                 // 1) preparer l'etat "relache"
	GPIO_SetOpenDrain(GPIOA, 10);           // 2) passer en drain ouvert
	GPIO_SetPullUp(GPIOA, 10);              //    + pull-up interne (en attendant la 4,7 kOhm)
	GPIO_SetMode(GPIOA, 10, OUTPUT_MODE);   // 3) enfin, passer en sortie
}

void GPIO_SetMode(GPIO_TypeDef* GPIO, uint16_t pin, uint8_t mode){
	GPIO->MODER &= ~(BITMASK_2BITS << (2 * pin));            // 1) effacer les 2 bits -> 00
	GPIO->MODER |=  ((mode & BITMASK_2BITS) << (2 * pin));   // 2) ecrire le mode
}

void GPIO_SetOpenDrain(GPIO_TypeDef* GPIO, uint16_t pin){
	GPIO->OTYPER |= (1 << pin);                              // 1 = drain ouvert
}

void GPIO_SetPullUp(GPIO_TypeDef* GPIO, uint16_t pin){
	GPIO->PUPDR &= ~(BITMASK_2BITS << (2 * pin));            // 1) effacer les 2 bits -> 00
	GPIO->PUPDR |=  (1 << (2 * pin));                        // 2) ecrire 01 = pull-up
}

void GPIO_SetPin(GPIO_TypeDef* GPIO, uint16_t pin){
	GPIO->ODR |= 1 << pin ;
}

void GPIO_ResetPin(GPIO_TypeDef* GPIO, uint16_t pin){
	GPIO->ODR &= ~(1 << pin) ;
}

void GPIO_TogglePin(GPIO_TypeDef* GPIO, uint16_t pin){
	GPIO->ODR ^= 1 << pin ;
}

uint16_t GPIO_ReadPin(GPIO_TypeDef* GPIO, uint16_t pin){
	if((GPIO->IDR & (1 << pin)) !=0)	return 1;
	else 								return 0;
}
