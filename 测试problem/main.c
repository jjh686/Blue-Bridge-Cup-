#include <STC15F2K60S2.H>
#include "Seg7.h"

void main()
{
	Seg_Init();
	Seg_SetNumber(2026);

	while (1)
	{
		Seg_Scan();
	}
}
