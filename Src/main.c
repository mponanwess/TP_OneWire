#include "app.h"

/* Application 1 : test du bus One-Wire a l'oscilloscope */
int main(void)
{
	APP_Init();

	while(1){
		APP_TestOneWire();
	}
}
