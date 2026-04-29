#include "KBD.h"

#define KEY_NO 0
#define KEY_DOWN 1
#define KEY_UP 2

u8 vKBD_KeyIO_State()
{
	u8 key_io = 0;
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

u8 vKBD_KeyIO_ThreeLine()
{
	u8 key_io = 0xff;
	P3=0xf0;P4=0xff;
	if(P44==0) key_io=0x70;
	if(P42==0) key_io=0xb0;
	if(P35==0) key_io=0xd0;
	if(P34==0) key_io=0xe0;
	
	P3=0x0f;P4=0x00;
	if(P33==0) key_io=key_io | 0x07;
	if(P32==0) key_io=key_io | 0x0b;
	if(P31==0) key_io=key_io | 0x0d;
	if(P30==0) key_io=key_io | 0x0e;
	
	return key_io;
}

u8 vKBD_Read_State()
{
	static u8 key_state = 0;
	u8 key_io = 0, key_return = 0;
	
	key_io = vKBD_KeyIO_State();
	switch(key_state)
	{
		case KEY_NO:
			if(key_io) key_state = KEY_DOWN;
		break;
		
		case KEY_DOWN:
			if(key_io)
			{
				key_state = KEY_UP;
				key_return = key_io;
			}
			else
				key_state = KEY_NO;
		break;
			
		case KEY_UP:
			if(key_io==0) key_state = KEY_NO;
		break;
	}
	return key_return;
}

unsigned char Trg_KBD;
unsigned char Cont_KBD;
void vKBD_Read_ThreeLine()
{
	unsigned char ReadData = vKBD_KeyIO_ThreeLine() ^ 0xff;
	Trg_KBD = ReadData & (ReadData ^ Cont_KBD);
	Cont_KBD = ReadData;
}
