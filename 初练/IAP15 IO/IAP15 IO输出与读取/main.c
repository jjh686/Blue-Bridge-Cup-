#include <STC15F2K60S2.H>

bit read_p21;
unsigned char read_p0;

void main(void)
{
	P1 = 0x5a;
	while(1)
	{
		read_p21 = P21;
		read_p0 = P0;
	}
}