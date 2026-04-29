#include <intrins.h>
#include "Delay.h"
#include "Device.h"
#include "system.h"

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
