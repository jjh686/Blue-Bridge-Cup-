#include "P2P0.h"

void P2P0_ctrl(u8 p2data,u8 p0data)
{
	P0=p0data;
	P2=p2data;
	P2=0;
}