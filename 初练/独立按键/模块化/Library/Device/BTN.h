#ifndef __BTN_H_
#define __BTN_H_
#include "system.h"

extern unsigned char Trg;
extern unsigned char Cont;

u8 vBTN_Read_State(void);
void vBTN_Read_ThreeLine( void );
	
#endif
