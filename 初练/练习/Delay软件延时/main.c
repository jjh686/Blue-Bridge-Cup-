#include <STC15F2K60S2.H>
#include "intrins.h"

void vDelay_Ms(unsigned int ms)		//@12.000MHz
{
	unsigned char i, j, k;

	for(k=0;k<ms;k++)
	{
		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}
}

void vDelay_Us(unsigned int us)		//@12.000MHz
{
	unsigned char k;	
	
	for(k=0;k<us;k++)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();		
	}
}

void vDelay_10us(unsigned int us)		//@12.000MHz
{
	unsigned char i, k;

	for(k=0;k<us;k++)
	{
		_nop_();
		_nop_();
		i = 27;
		while (--i);		
	}
}

void vDelay_100us(unsigned int us)		//@12.000MHz
{
	unsigned char i, j, k;

	for(k=0;k<us;k++)
	{
		i = 2;
		j = 39;
		do
		{
			while (--j);
		} while (--i);		
	}
}

void main(void)
{
	
	
	while(1)
	{
		P34 = 1;
		vDelay_Ms(1);
		vDelay_100us(5);
		P34 = 0;
		vDelay_100us(5);
	}
}
