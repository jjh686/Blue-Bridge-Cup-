#ifndef __DS1302_H
#define __DS1302_H

#include "system.h"

#define dectobcd(dec) (dec/10*16)+(dec%10)
#define bcdtodec(bcd) (bcd/16*10)+(bcd%16)

unsigned char Read_Ds1302_Byte( unsigned char address );
void clock_set(u8 hour,u8 minute,u8 second);
	
#endif
