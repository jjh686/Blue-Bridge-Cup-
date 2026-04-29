#include <STC15F2K60S2.H>
#include <intrins.h>
#include "Delay.h"
#include "Device.h"

unsigned char led_ctrl[9];

void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);
}

void vLED_Running_L1_L8(void)
{
	static unsigned char led_shift=0;
	vDevice_Ctrl(0x80,~(0x01<<led_shift));
	led_shift++;
	if(led_shift==8)	led_shift=0;
}

void vLED_Running_L8_L1(void)
{
	static unsigned char led_shift;
	vDevice_Ctrl(0x80,~(0x80>>led_shift));
	led_shift = (led_shift + 1) % 8;
}

void vLED_Running_L2_L4_L6_L8(void)
{
	static unsigned char led_shift;
	vDevice_Ctrl(0x80,~(0x02<<led_shift));
	led_shift = (led_shift + 2) % 8;
}

void main(void)
{
	vSystem_Init();
	led_ctrl[1]=1;led_ctrl[3]=1;led_ctrl[7]=1;led_ctrl[8]=1;
	vDevice_Ctrl(0x80,~(led_ctrl[8]<<7 | led_ctrl[7]<<6 | led_ctrl[5]<<4 | led_ctrl[3]<<2 | led_ctrl[2]<<1 | led_ctrl[1]));
	while(1)
	{
		vLED_Running_L2_L4_L6_L8();
		vDelay_Ms(200);
	}
}
