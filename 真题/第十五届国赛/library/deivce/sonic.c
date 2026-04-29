#include "sonic.h"

void vSendwave()
{
	u8 i,j;
	for(i=0;i<8;i++)
	{
		P10=1;
		for(j=0;j<25;j++)
			_nop_();
		P10=0;
		for(j=0;j<25;j++)
			_nop_();
	}
}

u16 ucDist_Get()
{
	u16 dist_val;
	vSendwave();
	CH=0;CL=0;
	CR=1;
	while(P11==1&&CF==0);
	CR=0;
	dist_val=((CH<<8)|CL)*0.017;
	return dist_val;
}
