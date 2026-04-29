#include "sonic.h"

void Delay12us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

void vSend_Wave()
{
	u8 i;
	for(i=0;i<8;i++)
	{
		P10=1;
		Delay12us();
		P10=0;
		Delay12us();
	}
}

u16 ucDistance_Get()
{
	u16 dist;
	vSend_Wave();
	TH1=0;TL1=0;
	TR1=1;
	while(P11==1);
	TR1=0;
	dist=((TH1<<8)|TL1)*0.017;
	return dist;
}
