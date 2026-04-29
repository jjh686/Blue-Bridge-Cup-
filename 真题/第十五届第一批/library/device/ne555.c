#include "ne555.h"

void vCounter0_Init()
{
	TMOD|=0x05;
	TL0=0x00;
	TH0=0x00;
	TR0=1;
}
