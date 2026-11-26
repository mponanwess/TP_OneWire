#include "ds18b20.h"
#include "onewire.h"
#include "timer.h"

/* Le bus sur lequel est branche le capteur (memorise par DS18B20_Init) */
static struct ONEWIRE_Config* ds18b20_bus;

void DS18B20_Init(struct ONEWIRE_Config* config)
{
	ds18b20_bus = config;
}

/**
 * Lance une conversion puis lit la temperature (cours, paragraphe 1.4).
 * @return la temperature en degres C, ou DS18B20_ERROR si aucun capteur
 */
float DS18B20_GetTemperature(void)
{
	uint8_t lsb, msb;
	int16_t brut;

	/* --- Conversion de la temperature --- */
	if (ONEWIRE_Reset(ds18b20_bus) == 0)                 // (1) Reset
		return DS18B20_ERROR;                            //     personne n'a repondu
	ONEWIRE_WriteByte(ds18b20_bus, DS18B20_SKIP_ROM);    // (2) SKIP_ROM
	ONEWIRE_WriteByte(ds18b20_bus, DS18B20_CONVERT_T);   // (3) CONVERT_T

	SYSTICK_Delay(750);                                  // conversion : jusqu'a 750 ms

	/* --- Lecture de la temperature --- */
	ONEWIRE_Reset(ds18b20_bus);                          // (1) Reset
	ONEWIRE_WriteByte(ds18b20_bus, DS18B20_SKIP_ROM);    // (2) SKIP_ROM
	ONEWIRE_WriteByte(ds18b20_bus, DS18B20_READ_SCRATCHPAD); // (3) READ_SCRATCHPAD
	lsb = ONEWIRE_ReadByte(ds18b20_bus);                 // (4) octet de poids faible
	msb = ONEWIRE_ReadByte(ds18b20_bus);                 //     octet de poids fort

	brut = (int16_t)((msb << 8) | lsb);                  // recoller les 2 octets
	return brut / 16.0f;                                 // 1 unite = 1/16 de degre
}

/**
 * Lit le code ROM de 64 bits du capteur (commande READ_ROM).
 * Ne fonctionne que s'il n'y a qu'UN seul capteur sur le bus.
 * @param rom : tableau de 8 octets rempli par la fonction
 *              rom[0] = code famille, rom[1..6] = numero de serie, rom[7] = CRC
 * @return 1 si un capteur a repondu, 0 sinon
 */
uint8_t DS18B20_ReadRom(uint8_t rom[8])
{
	uint8_t i;

	if (ONEWIRE_Reset(ds18b20_bus) == 0)                 // (1) Reset
		return 0;                                        //     personne n'a repondu

	ONEWIRE_WriteByte(ds18b20_bus, DS18B20_READ_ROM);    // (2) READ_ROM

	for (i = 0; i < 8; i++)                              // (3) lire les 8 octets
		rom[i] = ONEWIRE_ReadByte(ds18b20_bus);

	return 1;
}
