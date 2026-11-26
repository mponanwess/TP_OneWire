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
	uint32_t debut, duree_ms, t_delay0, t_delay6, t_gpio;

	FPU_Init();
	GPIO_Init();
	USART2_Init();
	SYSTICK_Init();
	TIM2_Init();

	while(1){
		/* Test 1 : 1 000 000 us avec TIM2, mesures en ms avec le SysTick */
		debut = SYSTICK_Get();
		TIM2_DelayUs(1000000);
		duree_ms = SYSTICK_Get() - debut;

		/* Test 2 : cout reel des appels, mesure avec TIM2 lui-meme (en us) */
		debut = TIM2->CNT;
		TIM2_DelayUs(0);
		t_delay0 = TIM2->CNT - debut;

		debut = TIM2->CNT;
		TIM2_DelayUs(6);
		t_delay6 = TIM2->CNT - debut;

		debut = TIM2->CNT;
		GPIO_SetPin(GPIOA, 10);
		t_gpio = TIM2->CNT - debut;

		printf("DelayUs(1000000) = %lu ms | DelayUs(0) = %lu us | DelayUs(6) = %lu us | GPIO = %lu us\r\n",
				duree_ms, t_delay0, t_delay6, t_gpio);

		GPIO_TogglePin(GPIOA, 5);
	}
}
