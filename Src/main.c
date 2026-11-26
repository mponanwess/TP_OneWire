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
	uint32_t debut, t0, t1, tr;
	uint8_t bit;

	FPU_Init();
	GPIO_Init();
	USART2_Init();
	SYSTICK_Init();
	TIM2_Init();

	while(1){
		debut = TIM2->CNT;  ONEWIRE_WriteBit0(&bus);        t0 = TIM2->CNT - debut;
		debut = TIM2->CNT;  ONEWIRE_WriteBit1(&bus);        t1 = TIM2->CNT - debut;
		debut = TIM2->CNT;  bit = ONEWIRE_ReadBit(&bus);    tr = TIM2->CNT - debut;

		printf("WriteBit0 : %lu us | WriteBit1 : %lu us | ReadBit : bit = %d (%lu us)\r\n",
				t0, t1, bit, tr);

		GPIO_TogglePin(GPIOA, 5);
		SYSTICK_Delay(1000);
	}
}
