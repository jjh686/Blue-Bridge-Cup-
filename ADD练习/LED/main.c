#include "system.h"

hextobin led_ctrl,buzzer_ctrl;

void rybr_ctrl()
{
	P2P0_ctrl(0xa0,0);
}

void main()
{
	rybr_ctrl();
	led_ctrl.hex=0x55;
	P2P0_ctrl(0x80,led_ctrl.hex);
	
	buzzer_ctrl.b.b4=1;
	P2P0_ctrl(0xa0,buzzer_ctrl.hex);
	while(1)
	{
		led_ctrl.b.b1=1;
		P2P0_ctrl(0x80,led_ctrl.hex);
		buzzer_ctrl.b.b4=0;
		P2P0_ctrl(0xa0,buzzer_ctrl.hex);
		Delay200ms();
		
		led_ctrl.b.b1=0;
		P2P0_ctrl(0x80,led_ctrl.hex);
		buzzer_ctrl.b.b4=1;
		P2P0_ctrl(0xa0,buzzer_ctrl.hex);
		Delay200ms();	
	}
}
