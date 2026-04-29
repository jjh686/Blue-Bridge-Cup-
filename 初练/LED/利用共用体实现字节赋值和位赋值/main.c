#include <STC15F2K60S2.H>
#include <intrins.h>
#include "Delay.h"
#include "Device.h"

typedef struct
{
	unsigned char b0 : 1;
	unsigned char b1 : 1;
	unsigned char b2 : 1;
	unsigned char b3 : 1;
	unsigned char b4 : 1;
	unsigned char b5 : 1;
	unsigned char b6 : 1;
	unsigned char b7 : 1;
}bits;

typedef union
{
	unsigned char hex;
	bits b;
}HexToBin;

HexToBin led_ctrl,buzzer_ctrl;

void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);
}

void main(void)
{
	vSystem_Init();
	led_ctrl.hex = 0x55;
	vDevice_Ctrl(0x80,led_ctrl.hex);
	
	buzzer_ctrl.b.b4 = 1;
	vDevice_Ctrl(0xa0,buzzer_ctrl.hex);
	while(1)
	{
		led_ctrl.b.b1 = 1;
		vDevice_Ctrl(0x80,led_ctrl.hex);
		
		buzzer_ctrl.b.b6 = 1;
		vDevice_Ctrl(0xa0,buzzer_ctrl.hex);
		vDelay_Ms(200);
		
		led_ctrl.b.b1 = 0;
		vDevice_Ctrl(0x80,led_ctrl.hex);
		
		buzzer_ctrl.b.b6 = 0;
		vDevice_Ctrl(0xa0,buzzer_ctrl.hex);		
		vDelay_Ms(200);		
	}
}
