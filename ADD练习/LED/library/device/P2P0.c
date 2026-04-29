#include "P2P0.h"

void P2P0_ctrl(unsigned char P2data,unsigned char P0data)
{
	P0=P0data;
	P2=P2data;
	P2=0;
}
