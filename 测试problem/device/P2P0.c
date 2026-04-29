#include "P2P0.h"

void P2P0_Ctrl(unsigned char p2data,unsigned char p0data)
{
	P0=p0data;
	P2=(P2&0x1f)|p2data;
	P2&=0x1f;
}
