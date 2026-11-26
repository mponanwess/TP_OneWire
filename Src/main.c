#include <stdint.h>
#include <stm32f446xx.h>
#include <stdio.h>
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "timer.h"
#include "util.h"
#include "onewire.h"


int main(void)
{
	struct ONEWIRE_Config bus = { GPIOA, 10 };   // ligne One-Wire sur PA10 (D2)
	uint8_t presence;

	FPU_Init();
	GPIO_Init();
	USART2_Init();
	SYSTICK_Init();
	TIM2_Init();

	while(1){
		presence = ONEWIRE_Reset(&bus);
		printf("Reset : presence = %d\r\n", presence);

		GPIO_TogglePin(GPIOA, 5);
		SYSTICK_Delay(1000);
	}
}
