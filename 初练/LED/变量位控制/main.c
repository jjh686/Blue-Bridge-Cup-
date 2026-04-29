#include <STC15F2K60S2.H>
#include <intrins.h>
#include "Delay.h"
#include "Device.h"

unsigned char led_ctrl[9];

void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);
}

void main(void)
{
	vSystem_Init();
	led_ctrl[1]=1;led_ctrl[3]=1;led_ctrl[7]=1;led_ctrl[8]=1;
	vDevice_Ctrl(0x80,~(led_ctrl[8]<<7 | led_ctrl[7]<<6 | led_ctrl[5]<<4 | led_ctrl[3]<<2 | led_ctrl[2]<<1 | led_ctrl[1]));
	while(1)
	{
		
	}
}
