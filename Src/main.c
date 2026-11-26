#include "app.h"

/* Application 3 : carte d'identite du capteur (code ROM) */
int main(void)
{
	APP_Init();

	while(1){
		APP_Identite();
	}
}
