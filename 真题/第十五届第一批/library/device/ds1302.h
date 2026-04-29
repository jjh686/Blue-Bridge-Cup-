#ifndef __DS1302_H_
#define __DS1302_H_

#include "system.h"

#define dectobcd(dec) (dec/10*16)+(dec%10)
#define bcdtodec(bcd) (bcd/16*10)+(bcd%16)

void vClock_Set(u8 hour,u8 minute,u8 second);
unsigned char Read_Ds1302_Byte ( unsigned char address );

#endif
