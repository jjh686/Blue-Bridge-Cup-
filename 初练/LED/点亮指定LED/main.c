#include <STC15F2K60S2.H>
#include <intrins.h>
#include "Delay.h"
#include "Device.h"

unsigned char buzzer_ctrl;
unsigned char relay_ctrl;


void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);
}

void main(void)
{
	vSystem_Init();
	vDevice_Ctrl(0x80,0x3a);
	vDevice_Ctrl(0x80,0xaa);
	while(1)
	{
		
	}
}
