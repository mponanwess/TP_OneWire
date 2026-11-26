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
	uint32_t debut, duree;
	uint8_t octet;

	FPU_Init();
	GPIO_Init();
	USART2_Init();
	SYSTICK_Init();
	TIM2_Init();

	while(1){
		debut = TIM2->CNT;
		ONEWIRE_WriteByte(&bus, 0xCC);
		duree = TIM2->CNT - debut;

		octet = ONEWIRE_ReadByte(&bus);

		printf("WriteByte(0xCC) : %lu us | ReadByte : 0x%02X\r\n", duree, octet);

		GPIO_TogglePin(GPIOA, 5);
		SYSTICK_Delay(1000);
	}
}
