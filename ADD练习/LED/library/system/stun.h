#ifndef __STUN_H_
#define __STUN_H_

#include "system.h"

typedef struct
{
	u8 b0:1;
	u8 b1:1;
	u8 b2:1;
	u8 b3:1;
	u8 b4:1;
	u8 b5:1;
	u8 b6:1;
	u8 b7:1;
}bits;

typedef union
{
	u8 hex;
	bits b;
}hextobin;


#endif
