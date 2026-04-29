#include <STC15F2K60S2.H>
#include <intrins.h>
#include "Delay.h"
#include "Device.h"

unsigned char buzzer_ctrl;
unsigned char relay_ctrl;


void vSystem_Init(void)
{
	buzzer_ctrl=0;
	relay_ctrl=1;
	vDevice_Ctrl(0xa0,(buzzer_ctrl<<6)|(relay_ctrl<<4));
}

void main(void)
{
	vSystem_Init();
	while(1)
	{
		buzzer_ctrl=1;
		vDevice_Ctrl(0xa0,(buzzer_ctrl<<6)|(relay_ctrl<<4));
		vDelay_Ms(500);
		buzzer_ctrl=0;
		vDevice_Ctrl(0xa0,(buzzer_ctrl<<6)|(relay_ctrl<<4));
		vDelay_Ms(500);
	}
}
