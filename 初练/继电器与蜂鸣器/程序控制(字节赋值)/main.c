#include <STC15F2K60S2.H>
#include <intrins.h>
#include "Delay.h"

sbit bRelay = P0^4;
sbit bBuzzer = P0^6;

void vSystem_Init(void)
{
	bRelay=1;
	bBuzzer=0;
	P27=1;P26=0;P25=1;
	P27=0;P26=0;P25=0;
}

void main(void)
{
	vSystem_Init();
	while(1)
	{
		
	}
}
