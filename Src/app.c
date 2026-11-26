#include <stdint.h>
#include <stdio.h>
#include "app.h"
#include "gpio.h"
#include "usart.h"
#include "timer.h"
#include "util.h"
#include "onewire.h"
#include "ds18b20.h"

#define SEUIL_ALARME	28.0f		// degres C : au-dessus, la LED s'allume

/* Le seul endroit du projet ou l'on dit sur quelle broche est le bus */
static struct ONEWIRE_Config bus = { GPIOA, 10 };   // PA10 (D2)

void APP_Init(void)
{
	FPU_Init();
	GPIO_Init();
	USART2_Init();
	SYSTICK_Init();
	TIM2_Init();
	DS18B20_Init(&bus);                      // la couche 2 memorise le bus
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

/* ------------------------------------------------------------------
 * APPLICATION 2 : thermometre (cours, paragraphe 1.4) avec alarme
 * Affiche la temperature ; LED allumee au-dessus de SEUIL_ALARME.
 * ------------------------------------------------------------------ */
void APP_Thermometre(void)
{
	float temperature;

	temperature = DS18B20_GetTemperature();  // ~0,75 s (conversion)

	if (temperature == DS18B20_ERROR){
		printf("Aucun capteur detecte\r\n");
		GPIO_ResetPin(GPIOA, 5);             // LED eteinte
		SYSTICK_Delay(1000);
		return;
	}

	if (temperature > SEUIL_ALARME){
		GPIO_SetPin(GPIOA, 5);               // alarme : LED allumee
		printf("Temperature : %.2f C  ALARME (> %.1f C)\r\n", temperature, SEUIL_ALARME);
	}
	else{
		GPIO_ResetPin(GPIOA, 5);             // LED eteinte
		printf("Temperature : %.2f C\r\n", temperature);
	}

	SYSTICK_Delay(250);                      // une mesure par seconde environ
}

/* ------------------------------------------------------------------
 * APPLICATION 3 : carte d'identite du capteur (commande READ_ROM)
 * Affiche le code ROM de 64 bits et en deduit le type de capteur.
 * ------------------------------------------------------------------ */
void APP_Identite(void)
{
	uint8_t rom[8];                          // tableau de 8 octets
	uint8_t i;

	if (DS18B20_ReadRom(rom) == 0){
		printf("Aucun capteur detecte\r\n");
		SYSTICK_Delay(1000);
		return;
	}

	printf("Code ROM :");
	for (i = 0; i < 8; i++)
		printf(" %02X", rom[i]);             // les 8 octets, dans l'ordre de reception
	printf("\r\n");

	printf("  Famille : 0x%02X -> ", rom[0]);
	if (rom[0] == 0x28)
		printf("DS18B20\r\n");
	else if (rom[0] == 0x10)
		printf("DS1820 / DS18S20\r\n");
	else
		printf("inconnue\r\n");

	printf("  Numero de serie :");
	for (i = 6; i >= 1; i--)                 // octet de poids fort en premier
		printf(" %02X", rom[i]);
	printf("\r\n");

	printf("  CRC : 0x%02X\r\n\r\n", rom[7]);

	SYSTICK_Delay(2000);
}


