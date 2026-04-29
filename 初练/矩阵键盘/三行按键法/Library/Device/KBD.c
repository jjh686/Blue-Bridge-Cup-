#include "KBD.h"

#define KEY_NO 0
#define KEY_DOWN 1
#define KEY_UP 2
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
