#include "system.h"

void main()
{
	while(1)
	{
		P34=1;
		Delay1ms(1);
		Delay100us(5);
		P34=0;
		Delay100us(5);
	}
}
