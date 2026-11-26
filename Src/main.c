#include <stdint.h>
#include <stm32f446xx.h>
#include <stdio.h>
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "timer.h"
#include "util.h"


int main(void)
{
	FPU_Init();
	GPIO_Init();
	USART2_Init();
	SYSTICK_Init();

	/* Test 1 : les registres de PA10 */
	printf("MODER  = %08lX\r\n", GPIOA->MODER);
	printf("OTYPER = %08lX\r\n", GPIOA->OTYPER);
	printf("PUPDR  = %08lX\r\n", GPIOA->PUPDR);

	/* Test 2 : ecrire sur la ligne (ODR) puis relire le fil (IDR) */
	GPIO_ResetPin(GPIOA, 10);               // le STM32 tire la ligne a 0
	SYSTICK_Delay(1);
	printf("Ligne tiree    : PA10 = %d\r\n", GPIO_ReadPin(GPIOA, 10));

	GPIO_SetPin(GPIOA, 10);                 // le STM32 relache la ligne
	SYSTICK_Delay(1);
	printf("Ligne relachee : PA10 = %d\r\n", GPIO_ReadPin(GPIOA, 10));

	while(1){
		SYSTICK_Delay(1000);
		GPIO_TogglePin(GPIOA, 5);
		printf("Hello\r\n");
	}
}
