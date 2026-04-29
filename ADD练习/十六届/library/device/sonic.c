#include "sonic.h"

//void Delay12us()		//@12.000MHz
//{
//	unsigned char i;

//	_nop_();
//	_nop_();
//	i = 33;
//	while (--i);
//}

//void vSend_Wave()
//{
//	u8 i;
//	for(i=0;i<8;i++)
//	{
//		P10=1;
//		Delay12us();
//		P10=0;
//		Delay12us();
//	}
//	
//}

void vSend_Wave_nop()
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

//u16 ucDistance_Get()
//{
//	u16 distance_val;
//	vSend_Wave();
//	TL1=0;TH1=0;
//	TR1=1;
//	while(P11==1);
//	TR1=0;
//	distance_val=((TH1<<8)|TL1)*0.017;
//	return distance_val;
//}

u16 ucDistance_Get_PAC()
{
	u16 distance_val;
	vSend_Wave_nop();
	CH=0;CL=0;
	CR=1;
	while(P11==1&&CF==0);
	CR=0;
	distance_val=((CH<<8)|CL)*0.017;
	return distance_val;
}
