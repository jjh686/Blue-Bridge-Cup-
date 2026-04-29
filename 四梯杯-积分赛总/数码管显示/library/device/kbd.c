#include "kbd.h"

#define KEY_NO 0
#define KEY_DOWN 1
#define KEY_UP 2

u8 ucBTN_KeyIO_State()
{
	u8 key_io=0;
	P3 = P3 & 0x0f; 
	if(P3!=0x0f)
	{ 
		if(P3==0x0e) key_io = 7;  	//S7
		if(P3==0x0d) key_io = 6;  	//S6
		if(P3==0x0b) key_io = 5;  	//S5
		if(P3==0x07) key_io = 4;  	//S4 
	} 
	return key_io;
}

u8 ucBTN_Read_State()
{
	static u8 key_state;
	u8 key_io=0,key_return=0;
	key_io=ucBTN_KeyIO_State();
	switch(key_state)
	{
		case KEY_NO:
			if(key_io) key_state=KEY_DOWN;
		break;
		
		case KEY_DOWN:
			if(key_io) 
			{
				key_state=KEY_UP;
				key_return=key_io;
			}
			else
				key_state=KEY_NO;
		break;
			
		case KEY_UP:
				if(key_io==0) key_state=KEY_NO;
		break;	
	}
	return key_return;
}
