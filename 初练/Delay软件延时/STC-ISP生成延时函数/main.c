#include <STC15F2K60S2.H>
#include "intrins.h"

void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}

void main(void)
{
	
	while(1)
	{
		P34 = 1;
		Delay1ms();
		P34 = 0;
		Delay1ms();
	}
}



