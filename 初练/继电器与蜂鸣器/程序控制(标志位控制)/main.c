#include <STC15F2K60S2.H>
#include <intrins.h>
#include "Delay.h"

unsigned char buzzer_ctrl;
unsigned char relay_ctrl;

void vSystem_Init(void)
{
	buzzer_ctrl=0;
	relay_ctrl=1;
	P0=(buzzer_ctrl<<6)|(relay_ctrl<<4);
	P2=0xa0;
	P2=0;
}

void main(void)
{
	vSystem_Init();
	while(1)
	{
		
	}
}
