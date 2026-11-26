#ifndef DS18B20_H_
#define DS18B20_H_

#include <stdint.h>
#include "onewire.h"

/* Commandes du DS18B20 (cours, paragraphe 1.4) */
#define DS18B20_SKIP_ROM		0xCC	// ROM function : s'adresser au seul capteur du bus
#define DS18B20_CONVERT_T		0x44	// MEMORY function : lancer une conversion
#define DS18B20_READ_SCRATCHPAD	0xBE	// MEMORY function : lire la memoire du capteur

#define DS18B20_ERROR			(-999.0f)	// valeur renvoyee si aucun capteur ne repond

void    DS18B20_Init(struct ONEWIRE_Config* config);
float   DS18B20_GetTemperature(void);

#endif
