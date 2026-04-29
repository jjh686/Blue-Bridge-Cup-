#include "ne555.h"

void vCounter0_Init()
{
	TMOD|=0x05;
	TL0=0;
	TH0=0;
	TR0=1;
}
