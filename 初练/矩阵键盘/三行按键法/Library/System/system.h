#ifndef __SYSTEM_H_
#define __SYSTEM_H_

typedef char s8;
typedef unsigned char u8;
typedef int s16;
typedef unsigned int u16;
typedef unsigned long u32;

#include <STC15F2K60S2.H>
#include <intrins.h>
#include "Delay.h"
#include "Device.h"
#include "Timer.h"
#include "SMG.h"
#include "BTN.h"
#include "KBD.h"

typedef struct
{
	unsigned char b0 : 1;
	unsigned char b1 : 1;
	unsigned char b2 : 1;
	unsigned char b3 : 1;
	unsigned char b4 : 1;
	unsigned char b5 : 1;
	unsigned char b6 : 1;
	unsigned char b7 : 1;
}bits;

typedef union
{
	unsigned char hex;
	bits b;
}HexToBin;



#endif
