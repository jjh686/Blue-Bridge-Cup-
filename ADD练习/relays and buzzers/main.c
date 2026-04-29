#include "system.h"

u8 buzzer_ctrl;
u8 relay_ctrl;

void relaybuzzer_ctrl()
{
	buzzer_ctrl=0;
	relay_ctrl=1;
	P2P0_ctrl(0xa0,(buzzer_ctrl<<6)|(relay_ctrl<<4));
}

void main()
{
	relaybuzzer_ctrl();
	
	while(1)
	{
		relay_ctrl=0;
		P2P0_ctrl(0xa0,(relay_ctrl<<4));
		Delay500ms();
		relay_ctrl=1;
		P2P0_ctrl(0xa0,(relay_ctrl<<4));
		Delay500ms();
	}
}
