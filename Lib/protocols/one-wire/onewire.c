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

/**
 * Envoie un '0' : ligne a 0 pendant presque tout le creneau.
 * Reel : ~65 us a 0, creneau ~82 us (norme : 60 a 120 us).
 */
void ONEWIRE_WriteBit0(struct ONEWIRE_Config* config)
{
	GPIO_ResetPin(config->port, config->pin);        // tirer la ligne a 0
	TIM2_DelayUs(60);                                //    pendant 60 us
	GPIO_SetPin(config->port, config->pin);          // relacher
	TIM2_DelayUs(10);                                //    temps de recuperation
}

/**
 * Envoie un '1' : ligne a 0 tres brievement, puis relachee.
 * Cours : 6 us a 0. Reel : DelayUs(1) + surcout = ~6 us (norme : 1 a 15 us).
 */
void ONEWIRE_WriteBit1(struct ONEWIRE_Config* config)
{
	GPIO_ResetPin(config->port, config->pin);        // tirer la ligne a 0
	TIM2_DelayUs(1);                                 //    ~6 us en realite
	GPIO_SetPin(config->port, config->pin);          // relacher
	TIM2_DelayUs(64);                                //    jusqu'a la fin du creneau
}

/**
 * Envoie un symbole de lecture et lit la reponse de l'esclave.
 * Cours : 6 us a 0, puis 9 us, puis lecture, puis 55 us.
 * Reel : ~6 us a 0, lecture vers ~13 us (norme : avant 15 us).
 * @return le bit lu (0 ou 1)
 */
uint8_t ONEWIRE_ReadBit(struct ONEWIRE_Config* config)
{
	uint8_t bit;

	GPIO_ResetPin(config->port, config->pin);        // 1) tirer la ligne a 0
	TIM2_DelayUs(1);                                 //    ~6 us en realite
	GPIO_SetPin(config->port, config->pin);          // 2) relacher
	TIM2_DelayUs(1);                                 //    ~4 us en realite

	bit = GPIO_ReadPin(config->port, config->pin);   // 3) lire vers 13 us

	TIM2_DelayUs(55);                                // 4) fin du creneau

	return bit;
}
