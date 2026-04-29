#include "kbd.h"

u8 KBD_Keyio_state()
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

u8 KBD_Keyio_state_zh()
{
	u8 key_io_zh=0xff;
	P3=0xf0;P4=0xff;
	if(P44==0) key_io_zh=0x70;
	if(P42==0) key_io_zh=0xb0;
	if(P35==0) key_io_zh=0xd0;
	if(P34==0) key_io_zh=0xe0;
	
	P3=0x0f;P4=0x00;
	if(P33==0) key_io_zh|=0x07;
	if(P32==0) key_io_zh|=0x0b;
	if(P31==0) key_io_zh|=0x0d;
	if(P30==0) key_io_zh|=0x0e;
	
	return key_io_zh;
}

#define KEY_NO 0
#define KEY_DOWN 1
#define KEY_UP 2

u8 KBD_Keyio_Read()
{
	static u8 key_state=0;
	u8 key_io=0,key_val=0;
	key_io=KBD_Keyio_state();
	switch(key_state)
	{
		case KEY_NO:
			if(key_io!=0) key_state=KEY_DOWN;
		break;
		case KEY_DOWN:
			if(key_io!=0)
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
