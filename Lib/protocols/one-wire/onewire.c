#include "onewire.h"
#include "gpio.h"
#include "timer.h"

/* Remarque : a 16 MHz, chaque appel de fonction coute ~3 us (mesure etape 7).
 * Certains delais sont reduits pour que les durees REELLES respectent la norme. */

/**
 * Envoie le symbole de reset.
 * @return 1 si au moins un device a repondu (presence), 0 sinon
 */
uint8_t ONEWIRE_Reset(struct ONEWIRE_Config* config)
{
	uint8_t presence;

	GPIO_ResetPin(config->port, config->pin);        // 1) tirer la ligne a 0
	TIM2_DelayUs(480);                               //    pendant 480 us

	GPIO_SetPin(config->port, config->pin);          // 2) relacher la ligne
	TIM2_DelayUs(65);                                //    lecture vers 70 us en realite (65 + ~5 us)

	if (GPIO_ReadPin(config->port, config->pin) == 0)  // 3) lire : 0 = un device tire la ligne
		presence = 1;
	else
		presence = 0;

	TIM2_DelayUs(410);                               // 4) fin du creneau de reset

	return presence;
}
