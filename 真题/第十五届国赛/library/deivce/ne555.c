#include "ne555.h"

void vCounter0_init()
{
	TMOD|=0x05;
	TH0=0;
	TL0=0;
	TR0=1;
}
