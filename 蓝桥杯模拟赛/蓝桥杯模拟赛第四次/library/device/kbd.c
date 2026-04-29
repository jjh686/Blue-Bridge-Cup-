#include "kbd.h"

u8 ucKBD_Keyio_State()
{
	u8 key_io=0;
	P3=0xf0;P4=0xff;
	if(P44==0) key_io=4;
	if(P42==0) key_io=8;
	if(P35==0) key_io=12;
	if(P34==0) key_io=16;
	
	P3=0x0f;P4=0x00;
	if(P33==0) key_io+=0;
	if(P32==0) key_io+=1;
	if(P31==0) key_io+=2;
	if(P30==0) key_io+=3;
	
	return key_io;
}

#define KEY_NO 0
#define KEY_DOWN 1
#define KEY_UP 2

u8 ucKBD_Read_State()
{
	static u8 key_state=KEY_NO;
	u8 key_io,key_val;
	key_io=ucKBD_Keyio_State();
	switch(key_state)
	{
		case KEY_NO:
			if(key_io) key_state=KEY_DOWN;
		break;
		case KEY_DOWN:
			if(key_io)
			{
				key_state=KEY_UP;
				key_val=key_io;
			}
			else
				key_state=KEY_NO;
			break;
		case KEY_UP:
			if(key_io==0) key_state=KEY_NO;
		break;
	}
	return key_val;
}
