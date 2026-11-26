#include "app.h"

/* Application 2 : thermometre avec alarme */
int main(void)
{
	APP_Init();

	while(1){
		APP_Thermometre();
	}
}
