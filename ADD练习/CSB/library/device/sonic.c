#include "sonic.h"

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

u16 uiDistance_Get()
{
	u16 dist;
	vSend_Wave();
	TH1=0;TL1=0;
	TR1=1;
	while((P11==1)&&(TF1==0));
	TR1=0;
	if(TF1==1)
	{
		TF1=0;
	}
	else
	{
		dist=(TH1<<8|TL1)*0.017;
	}
	return dist;
}
