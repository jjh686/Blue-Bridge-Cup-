#include "delay.h"

void Delay1ms(unsigned int ms)		//@12.000MHz
{
	unsigned char i, j;
	unsigned int k;

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