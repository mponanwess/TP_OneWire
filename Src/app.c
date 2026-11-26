#include <stdint.h>
#include <stdio.h>
#include "app.h"
#include "gpio.h"
#include "usart.h"
#include "timer.h"
#include "util.h"
#include "onewire.h"

/* Le seul endroit du projet ou l'on dit sur quelle broche est le bus */
static struct ONEWIRE_Config bus = { GPIOA, 10 };   // PA10 (D2)

void APP_Init(void)
{
	FPU_Init();
	GPIO_Init();
	USART2_Init();
	SYSTICK_Init();
	TIM2_Init();
}

/* ------------------------------------------------------------------
 * APPLICATION 1 : test du bus a l'oscilloscope (sans composant)
 * Une trame qui contient chaque symbole One-Wire une fois,
 * separes par 100 us de silence. A observer sur D2.
 * ------------------------------------------------------------------ */
void APP_TestOneWire(void)
{
	uint8_t presence, bit, octet;

	presence = ONEWIRE_Reset(&bus);          // A : Reset         (~485 us a 0)
	TIM2_DelayUs(100);

	ONEWIRE_WriteBit0(&bus);                 // B : ecrire un '0' (~65 us a 0)
	TIM2_DelayUs(100);

	ONEWIRE_WriteBit1(&bus);                 // C : ecrire un '1' (~6 us a 0)
	TIM2_DelayUs(100);

	bit = ONEWIRE_ReadBit(&bus);             // D : lire un bit   (~6 us a 0)
	TIM2_DelayUs(100);

	ONEWIRE_WriteByte(&bus, 0xCC);           // E : l'octet 0xCC (8 creneaux)
	TIM2_DelayUs(100);

	octet = ONEWIRE_ReadByte(&bus);          // F : lire un octet (8 creneaux courts)

	printf("presence=%d  bit=%d  octet=0x%02X\r\n", presence, bit, octet);

	GPIO_TogglePin(GPIOA, 5);                // la LED montre que la boucle tourne
	SYSTICK_Delay(10);                       // 10 ms de silence avant la trame suivante
}
