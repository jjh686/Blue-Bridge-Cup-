#include <STC15F2K60S2.H>
#include <intrins.h>
#include "Delay.h"

void main(void)
{
	
	while(1)
	{
		P34 = 1;
		vDelay_Ms(1);vDelay_100us(5);
		P34 = 0;
		vDelay_100us(5);			// µœ÷—” ±0.5ms
	}
}
